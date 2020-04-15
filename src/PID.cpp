#include "PID.h"
#include <iostream>
#include <cmath>

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
	Kp = Kp_;
	Ki = Ki_;
	Kd = Kd_;

	have_data = 0;
	d_error = 0;
	history_size = 10;
}

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on cte.
   */
	std::cout << "CTE: " << cte << ", ";

	// Calculate P error
	p_error = Kp * cte;

	// Calculate D error and update "last seen CTE"
	if (have_data) {
		double diff_cte = cte - prev_cte.back();

		// Report CTE spikes (because they happen often)
		// In the case that CTE spikes, the D error becomes a liability, so we
		// take it out and effectively just use a PI controller for spikes
		if (std::abs(diff_cte) > 0.1) {
			std::cout << "---------------------------" << std::endl
								<< "\t\tCTE DISCONTINUOUS > 0.1" << std::endl
								<< "\t\t-------------------------\t";
			p_error *= 2;
			d_error = 0;
		}
		else
			d_error = Kd * diff_cte;
	}

	// Data management - only keep 10pts to avoid the PID controller "remembering"
	// unnecessary bias
	prev_cte.push_back(cte);
	if (have_data > history_size)
		prev_cte.pop_front();
	else
		have_data++;

	// Calculate I error
	i_error = 0;
	for (double e : prev_cte)
		i_error += e;			// sum previous CTE's (up to last 50 only)
	i_error *= Ki;

}

double PID::TotalError() {
  /**
   * TODO: Calculate and return the total error
   */
	double total = p_error + i_error + d_error;

	// std::cout << ", P: " << p_error
	// 		      << ", I: " << i_error
	// 					<< ", D: " << d_error << std::endl;

	//if (std::abs(total) > 2)
	//	total /= 20.0;		// Attempt to reduce magnitude of spikes

  return total;
}
