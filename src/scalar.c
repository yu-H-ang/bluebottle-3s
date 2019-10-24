/*******************************************************************************
 ********************************* BLUEBOTTLE **********************************
 *******************************************************************************
 *
 *  Copyright 2015 - 2016 Yayun Wang, The Johns Hopkins University
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  Please contact the Johns Hopkins University to use Bluebottle for
 *  commercial and/or for-profit applications.
 ******************************************************************************/

#include "scalar.h"

BC_s bc_s;
BC_s *_bc_s;
real s_D;
real s_k;
real s_init;
real s_init_rand;
real s_alpha;
real lamb_cut_scalar;
int SCALAR;
int s_ncoeffs_max;

real *s0;
real *s;
real *s_conv0;
real *s_conv;
real *s_diff0;
real *s_diff;

real *_s0;
real *_s;
real *_s_conv0;
real *_s_conv;
real *_s_diff0;
real *_s_diff;

void scalar_init_fields(void)
{
  // rng seed must be different for processes
  //srand(time(NULL) + rank);
  for (int i = 0; i < dom[rank].Gcc.s3b; i++) {
    //s[i] = s_init + s_init_rand * (2. * rand() / (real)RAND_MAX - 1.);
    //s0[i] = s[i];
    s_conv[i] = 0.0;
    s_conv0[i] = 0.0;
    s_diff[i] = 0.0;
    s_diff0[i] = 0.0;
  }

  int i, j, k;
  int ii, jj, kk;
  int _c, c;

  for (i = dom[rank].Gcc._isb; i <= dom[rank].Gcc._ieb; i++) {
    for (j = dom[rank].Gcc._jsb; j <= dom[rank].Gcc._jeb; j++) {
      for (k = dom[rank].Gcc._ksb; k <= dom[rank].Gcc._keb; k++) {
        ii = i + dom[rank].Gcc.isb;
        jj = j + dom[rank].Gcc.jsb;
        kk = k + dom[rank].Gcc.ksb;
        _c = GCC_LOC(i, j, k, dom[rank].Gcc.s1b, dom[rank].Gcc.s2b);
        c = GCC_LOC(ii, jj, kk, DOM.Gcc.s1b, DOM.Gcc.s2b);
        // rng being seeded according to global location
        srand(c);
        s[_c] = s_init + s_init_rand * (2. * rand() / (real)RAND_MAX - 1.);
        s0[_c] = s[_c];
      }
    }
  }
}

void scalar_part_init(void)
{
  for (int i = 0; i < nparts; i++) {

    parts[i].srs = parts[i].srs * parts[i].r;
    parts[i].q = 0.0;

    parts[i].sncoeff = 0;
    // for each n, -n <= m <= n
    for(int j = 0; j <= parts[i].sorder; j++) {
      parts[i].sncoeff += 2*j + 1;
    }
    if(parts[i].sncoeff > S_MAX_COEFFS) {
      printf("Maximum order is 4.");
      exit(EXIT_FAILURE);
    }

    for (int j = 0; j < S_MAX_COEFFS; j++) {
      parts[i].anm_re[j] = 0.;
      parts[i].anm_im[j] = 0.;
      parts[i].anm_re0[j] = 0.;
      parts[i].anm_im0[j] = 0.;
    }
  }
}

void mpi_send_s_psums_i(void)
{
  // MPI Info
  MPI_Info no_locks;
  MPI_Info_create(&no_locks);
  MPI_Info_set(no_locks, "no_locks", "true");
  MPI_Info_set(no_locks, "same_disp_unit", "true");

  // Open MPI Windows for _recv_psums_{e,w}
  // Reuse windows from particle communication
  int n_scalar_prods = 2;
  int npsums = n_scalar_prods * s_ncoeffs_max;
  MPI_Win_create(_sum_recv_e, nparts_recv[EAST] * npsums * sizeof(real),
    sizeof(real), MPI_INFO_NULL, MPI_COMM_WORLD, &parts_recv_win_e);
  MPI_Win_create(_sum_recv_w, nparts_recv[WEST] * npsums * sizeof(real),
    sizeof(real), MPI_INFO_NULL, MPI_COMM_WORLD, &parts_recv_win_w);

  // Fence and put _sum_send -> _sum_recv
  MPI_Win_fence(0, parts_recv_win_e);
  MPI_Win_fence(0, parts_recv_win_w);

  MPI_Put(_sum_send_e, nparts_send[EAST] * npsums, mpi_real, dom[rank].e,
    0, nparts_send[EAST] * npsums, mpi_real, parts_recv_win_w);
  MPI_Put(_sum_send_w, nparts_send[WEST] * npsums, mpi_real, dom[rank].w,
    0, nparts_send[WEST] * npsums, mpi_real, parts_recv_win_e);

  MPI_Win_fence(0, parts_recv_win_e);
  MPI_Win_fence(0, parts_recv_win_w);

  // Free
  MPI_Win_free(&parts_recv_win_e);
  MPI_Win_free(&parts_recv_win_w);

  // Free the info we provided
  MPI_Info_free(&no_locks);
}

void mpi_send_s_psums_j(void)
{
  // MPI Info
  MPI_Info no_locks;
  MPI_Info_create(&no_locks);
  MPI_Info_set(no_locks, "no_locks", "true");
  MPI_Info_set(no_locks, "same_disp_unit", "true");

  // Open MPI Windows for _recv_psums_{e,w}
  // Reuse windows from particle communication
  int n_scalar_prods = 2;
  int npsums = n_scalar_prods * s_ncoeffs_max;
  MPI_Win_create(_sum_recv_n, nparts_recv[NORTH] * npsums * sizeof(real),
    sizeof(real), MPI_INFO_NULL, MPI_COMM_WORLD, &parts_recv_win_n);
  MPI_Win_create(_sum_recv_s, nparts_recv[SOUTH] * npsums * sizeof(real),
    sizeof(real), MPI_INFO_NULL, MPI_COMM_WORLD, &parts_recv_win_s);

  // Fence and put _sum_send -> _sum_recv
  MPI_Win_fence(0, parts_recv_win_n);
  MPI_Win_fence(0, parts_recv_win_s);

  MPI_Put(_sum_send_n, nparts_send[NORTH] * npsums, mpi_real, dom[rank].n,
          0, nparts_send[NORTH] * npsums, mpi_real, parts_recv_win_s);
  MPI_Put(_sum_send_s, nparts_send[SOUTH] * npsums, mpi_real, dom[rank].s,
          0, nparts_send[SOUTH] * npsums, mpi_real, parts_recv_win_n);

  MPI_Win_fence(0, parts_recv_win_n);
  MPI_Win_fence(0, parts_recv_win_s);

  // Free
  MPI_Win_free(&parts_recv_win_n);
  MPI_Win_free(&parts_recv_win_s);

  // Free the info we provided
  MPI_Info_free(&no_locks);
}

void mpi_send_s_psums_k(void)
{
  // MPI Info
  MPI_Info no_locks;
  MPI_Info_create(&no_locks);
  MPI_Info_set(no_locks, "no_locks", "true");
  MPI_Info_set(no_locks, "same_disp_unit", "true");

  // Open MPI Windows for _recv_psums_{e,w}
  // Reuse windows from particle communication
  int n_scalar_prods = 2;
  int npsums = n_scalar_prods * s_ncoeffs_max;
  MPI_Win_create(_sum_recv_t, nparts_recv[TOP] * npsums * sizeof(real),
    sizeof(real), MPI_INFO_NULL, MPI_COMM_WORLD, &parts_recv_win_t);
  MPI_Win_create(_sum_recv_b, nparts_recv[BOTTOM] * npsums * sizeof(real),
    sizeof(real), MPI_INFO_NULL, MPI_COMM_WORLD, &parts_recv_win_b);

  // Fence and put _sum_send -> _sum_recv
  MPI_Win_fence(0, parts_recv_win_t);
  MPI_Win_fence(0, parts_recv_win_b);

  MPI_Put(_sum_send_t, nparts_send[TOP] * npsums, mpi_real, dom[rank].t,
          0, nparts_send[TOP] * npsums, mpi_real, parts_recv_win_b);
  MPI_Put(_sum_send_b, nparts_send[BOTTOM] * npsums, mpi_real, dom[rank].b,
    0, nparts_send[BOTTOM] * npsums, mpi_real, parts_recv_win_t);

  MPI_Win_fence(0, parts_recv_win_t);
  MPI_Win_fence(0, parts_recv_win_b);

  // Free
  MPI_Win_free(&parts_recv_win_t);
  MPI_Win_free(&parts_recv_win_b);

  // Free the info we provided
  MPI_Info_free(&no_locks);
}
