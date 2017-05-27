/**
 * @mainpage Introduction
 *
 * @section purpose Purpose
 *
 * Implements a cubic spline interpolation interface for evalresp.
 *
 * @section history History
 *
 * Written by <a href="http://www.isti.com/">Instrumental Software
 * Technologies, Inc.</a> (ISTI) in 2017.
 */

/**
 * @defgroup evalresp_spline evalresp Private Spline Interpolation Interface
 * @ingroup evalresp
 * @brief Private cubic spline interpolation interface for evalresp.
 */

/**
 * @file
 * @brief This file contains declarations and global structures for evalresp
 *        cubic spline interpolation.
 */

/**
 * @private
 * @ingroup evalresp_spline
 * @brief Cubic spline interpolation.
 * @details Two "source" arrays (abscissa 'X' and ordinate 'Y' values) and an
 *          array of "new" abscissa values are given; an array of
 *          "destination" ordinate values is generated and returned. Any
 *          "new" abscissa values outside of the range of "source" abscissa
 *          values are ignored, resulting in a reduced-size "destination"
 *          ordinate array being generated and an error message being
 *          generated.
 * @param[in] num_points Number of points in given "source" arrays.
 * @param[in] t Abscissa "source" array of 'double' values.
 * @param[in] y Ordinate "source" array of 'double' values.
 * @param[in] tension Not used anymore.
 * @param[in] k Not used anymore.
 * @param[in] xvals_arr Array of "new" abscissa values to use with
 *                      interpolation ('double' values).
 * @param[in] num_xvals Number of entries in 'xvals_arr'.
 * @param[in] p_retvals_arr Reference to ordinate "destination" array of
 *                          'double' values generated via interpolation.
 * @param[in] p_num_retvals reference to number of values returned in
 *                          'p_retvals_arr' (will be less than 'num_xvals' if
 *                          any new abscissa values are out of range).
 * @return @c NULL if successful; an error message string if not.
 */
char * evr_spline(int num_points, double *t, double *y,
                  double tension, /* Not used anymore */
                  double k, /* Not used anymore */
                  double *xvals_arr, int num_xvals,
                  double **p_retvals_arr, int *p_num_retvals);
