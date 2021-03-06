/* Copyright 2018 Kristofer Björnson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @package TBTKQuantumTransportAtomToTransistor
 *  @file main.cpp
 *  @brief Exercise 7.1.a
 *
 *  Solution to exercise 7.1.a in the book "Quantum Transport: Atom to
 *  Transistor, S. Datta (2005)".
 *
 *  @author Kristofer Björnson
 */

#include "TBTK/Model.h"
#include "TBTK/Plotter.h"
#include "TBTK/PropertyExtractor/BlockDiagonalizer.h"
#include "TBTK/Range.h"
#include "TBTK/Solver/BlockDiagonalizer.h"
#include "TBTK/Streams.h"
#include "TBTK/UnitHandler.h"

#include <complex>

using namespace std;
using namespace TBTK;
using namespace Plot;

complex<double> i(0, 1);

int main(int argc, char **argv){
	//Set the natural units. Argument order: (charge, count, energy,
	//length, temperature, time).
	UnitHandler::setScales({"1 C", "1 pcs", "1 eV", "1 Ao", "1 K", "1 s"});

	//Parameters.
	double a = 1;
	double E_sa = -8.3431;
	double E_sc = -2.6569;
	double E_ss = -6.4513/4;
	double E_sapc = 4.48/4;
	double E_xx = 1.9546/4;
	double E_pa = 1.0414;
	double E_pc = 3.6686;
	double E_pasc = -5.7839/4;
	double E_s_apc = 4.8422/4;
	double E_xy = 5.0779/4;
	double E_s_a = 8.5914;
	double E_s_c = 6.7386;
	double E_pas_c = -4.8077/4;

	Vector3d d1({0, a/2, a/2});
	Vector3d d2({a/2, 0, a/2});
	Vector3d d3({a/2, a/2, 0});

	//Setup the model.
	Model model;
	for(int n = 0; n < 200; n++){
		Vector3d k;
		if(n < 100)
			k = Vector3d({M_PI*(1 - n/100.), M_PI*(1 - n/100.), M_PI*(1 - n/100.)});
		else
			k = Vector3d({2*M_PI*(n-100)/100., 0, 0});

		double kd1 = Vector3d::dotProduct(k, d1);
		double kd2 = Vector3d::dotProduct(k, d2);
		double kd3 = Vector3d::dotProduct(k, d3);

		complex<double> g_0 = (1. + exp(-i*kd1) + exp(-i*kd2) + exp(-i*kd3))/4.;
		complex<double> g_1 = (1. + exp(-i*kd1) - exp(-i*kd2) - exp(-i*kd3))/4.;
		complex<double> g_2 = (1. - exp(-i*kd1) + exp(-i*kd2) - exp(-i*kd3))/4.;
		complex<double> g_3 = (1. - exp(-i*kd1) - exp(-i*kd2) + exp(-i*kd3))/4.;

		//Diagonal entries.
		model << HoppingAmplitude(E_sa, {n, 0}, {n, 0});
		model << HoppingAmplitude(E_sc, {n, 1}, {n, 1});
		model << HoppingAmplitude(E_pa, {n, 2}, {n, 2});
		model << HoppingAmplitude(E_pa, {n, 3}, {n, 3});
		model << HoppingAmplitude(E_pa, {n, 4}, {n, 4});
		model << HoppingAmplitude(E_pc, {n, 5}, {n, 5});
		model << HoppingAmplitude(E_pc, {n, 6}, {n, 6});
		model << HoppingAmplitude(E_pc, {n, 7}, {n, 7});
		model << HoppingAmplitude(E_s_a, {n, 8}, {n, 8});
		model << HoppingAmplitude(E_s_c, {n, 9}, {n, 9});

		//Off-diagonal entries.
		model << HoppingAmplitude(4*E_ss*g_0, {n, 0}, {n, 1}) + HC;
		model << HoppingAmplitude(4*E_sapc*g_1, {n, 0}, {n, 5}) + HC;
		model << HoppingAmplitude(4*E_sapc*g_2, {n, 0}, {n, 6}) + HC;
		model << HoppingAmplitude(4*E_sapc*g_3, {n, 0}, {n, 7}) + HC;
		model << HoppingAmplitude(4*E_pasc*conj(g_1), {n, 1}, {n, 2}) + HC;
		model << HoppingAmplitude(4*E_pasc*conj(g_2), {n, 1}, {n, 3}) + HC;
		model << HoppingAmplitude(4*E_pasc*conj(g_3), {n, 1}, {n, 4}) + HC;
		model << HoppingAmplitude(4*E_xx*g_0, {n, 2}, {n, 5}) + HC;
		model << HoppingAmplitude(4*E_xy*g_3, {n, 2}, {n, 6}) + HC;
		model << HoppingAmplitude(4*E_xy*g_2, {n, 2}, {n, 7}) + HC;
		model << HoppingAmplitude(4*E_pas_c*g_1, {n, 2}, {n, 9}) + HC;
		model << HoppingAmplitude(4*E_xy*g_3, {n, 3}, {n, 5}) + HC;
		model << HoppingAmplitude(4*E_xx*g_0, {n, 3}, {n, 6}) + HC;
		model << HoppingAmplitude(4*E_xy*g_1, {n, 3}, {n, 7}) + HC;
		model << HoppingAmplitude(4*E_pas_c*g_2, {n, 3}, {n, 9}) + HC;
		model << HoppingAmplitude(4*E_xy*g_2, {n, 4}, {n, 5}) + HC;
		model << HoppingAmplitude(4*E_xy*g_1, {n, 4}, {n, 6}) + HC;
		model << HoppingAmplitude(4*E_xx*g_0, {n, 4}, {n, 7}) + HC;
		model << HoppingAmplitude(4*E_pas_c*g_3, {n, 4}, {n, 9}) + HC;
		model << HoppingAmplitude(4*E_s_apc*conj(g_1), {n, 5}, {n, 8}) + HC;
		model << HoppingAmplitude(4*E_s_apc*conj(g_2), {n, 6}, {n, 8}) + HC;
		model << HoppingAmplitude(4*E_s_apc*conj(g_3), {n, 7}, {n, 8}) + HC;
	}
	model.construct();

	//Setup and run the solver.
	Solver::BlockDiagonalizer solver;
	solver.setModel(model);
	solver.run();

	//Extract the eigenvalues.
	PropertyExtractor::BlockDiagonalizer propertyExtractor(solver);
	Array<double> eigenValues({10, 200});
	for(unsigned int n = 0; n < 200; n++){
		for(unsigned int e = 0; e < 10; e++)
			eigenValues[{e, n}] = propertyExtractor.getEigenValue({(int)n}, e);
	}

	//Calculate the approximation.
	double E_c = 0;
	double m_c = -1e-30;
	double hbar = UnitHandler::getHbarN();
	Array<double> h({200});
	for(unsigned int n = 0; n < 200; n++){
		Vector3d k;
		if(n < 100)
			k = Vector3d({M_PI*(1 - n/100.), M_PI*(1 - n/100.), M_PI*(1 - n/100.)});
		else
			k = Vector3d({2*M_PI*(n-100)/100., 0, 0});

		h[{n}] = E_c + pow(hbar*k.norm(), 2)/(2*m_c);
	}

	//Plot the eigenvalues.
	Plotter plotter;
	plotter.setLabelX("k");
	plotter.setLabelY("Energy (eV)");
	plotter.setHold(true);
	for(int n = 0; n < 10; n++)
		plotter.plot(eigenValues.getSlice({n, IDX_ALL}));

	//Plot the approximation.
	plotter.plot(
		h,
		Decoration({224, 32, 32}, Decoration::LineStyle::Line, 2)
	);
	plotter.save("figures/Spectrum.png");

	return 0;
}
