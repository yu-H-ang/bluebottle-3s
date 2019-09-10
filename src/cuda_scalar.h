#ifndef _CUDA_SCALAR_H
#define _CUDA_SCALAR_H

extern "C"
{
#include "bluebottle.h"
#include "scalar.h"
}

#include "cuda_particle.h"

__global__ void pack_s_parts_e(part_struct_scalar *send_parts, part_struct_scalar *parts,
  int *offset, int *bin_start, int *bin_count, int *part_ind);

__global__ void pack_s_parts_w(part_struct_scalar *send_parts, part_struct_scalar *parts,
  int *offset, int *bin_start, int *bin_count, int *part_ind);

__global__ void pack_s_parts_n(part_struct_scalar *send_parts, part_struct_scalar *parts,
  int *offset, int *bin_start, int *bin_count, int *part_ind);

__global__ void pack_s_parts_s(part_struct_scalar *send_parts, part_struct_scalar *parts,
  int *offset, int *bin_start, int *bin_count, int *part_ind);

__global__ void pack_s_parts_t(part_struct_scalar *send_parts, part_struct_scalar *parts,
  int *offset, int *bin_start, int *bin_count, int *part_ind);

__global__ void pack_s_parts_b(part_struct_scalar *send_parts, part_struct_scalar *parts,
  int *offset, int *bin_start, int *bin_count, int *part_ind);

__global__ void copy_central_bin_s_parts_i(part_struct_scalar *tmp_parts,
  part_struct_scalar *parts, int *bin_start, int *bin_count, int *part_ind, int *offset);

__global__ void copy_central_bin_s_parts_j(part_struct_scalar *tmp_parts,
  part_struct_scalar *parts, int *bin_start, int *bin_count, int *part_ind, int *offset);

__global__ void copy_central_bin_s_parts_k(part_struct_scalar *tmp_parts,
  part_struct_scalar *parts, int *bin_start, int *bin_count, int *part_ind, int *offset);

__global__ void copy_ghost_bin_s_parts(part_struct_scalar *tmp_parts,
  part_struct_scalar *recv_parts, int nparts_recv, int offset, int plane,
  dom_struct *DOM);

__global__ void BC_s_W_D(real *array, real bc_s);

__global__ void BC_s_W_N(real *array, real bc_s);

__global__ void BC_s_E_D(real *array, real bc_s);

__global__ void BC_s_E_N(real *array, real bc_s);

__global__ void BC_s_N_D(real *array, real bc_s);

__global__ void BC_s_N_N(real *array, real bc_s);

__global__ void BC_s_S_D(real *array, real bc_s);

__global__ void BC_s_S_N(real *array, real bc_s);

__global__ void BC_s_B_D(real *array, real bc_s);

__global__ void BC_s_B_N(real *array, real bc_s);

__global__ void BC_s_T_D(real *array, real bc_s);

__global__ void BC_s_T_N(real *array, real bc_s);

__global__ void forcing_boussinesq_x(real alpha, real gx, real s_init, real *s, real *fx);

__global__ void forcing_boussinesq_y(real alpha, real gy, real s_init, real *s, real *fy);

__global__ void forcing_boussinesq_z(real alpha, real gz, real s_init, real *s, real *fz);

__global__ void copy_subdom_parts_with_scalar(part_struct *tmp_parts, part_struct *parts,
  part_struct_scalar *tmp_s_parts, part_struct_scalar *s_parts,
  int *bin_start, int *bin_count, int *part_ind, int *bin_offset);

__global__ void scalar_solve(int *phase, real *s0, real *s,
  real *conv, real *diff, real *conv0, real *diff0,
  real *u0, real *v0, real *w0, real D, real dt, real dt0);

__global__ void scalar_check_nodes(part_struct *parts,
  part_struct_scalar *s_parts, BC_s *bc_s, dom_struct *DOM);

__global__ void scalar_interpolate_nodes(real *s, real *ss,
  part_struct *parts, part_struct_scalar *s_parts, BC_s *bc_s);

__global__ void scalar_lebedev_quadrature(part_struct *parts,
  part_struct_scalar *s_parts, int s_ncoeffs_max,
  real *ss, real *int_Ys_re, real *int_Ys_im);

__device__ real X_sn(int n, real theta, real phi,
  int pp, part_struct_scalar *s_parts);

__global__ void pack_s_sums_e(real *sum_send_e, int *offset, int *bin_start,
  int *bin_count, int *part_ind, int ncoeffs_max,
  real *int_Ys_re, real *int_Ys_im);

__global__ void pack_s_sums_w(real *sum_send_w, int *offset, int *bin_start,
  int *bin_count, int *part_ind, int ncoeffs_max,
  real *int_Ys_re, real *int_Ys_im);

__global__ void pack_s_sums_n(real *sum_send_n, int *offset, int *bin_start,
  int *bin_count, int *part_ind, int ncoeffs_max,
  real *int_Ys_re, real *int_Ys_im);

__global__ void pack_s_sums_s(real *sum_send_s, int *offset, int *bin_start,
  int *bin_count, int *part_ind, int ncoeffs_max,
  real *int_Ys_re, real *int_Ys_im);

__global__ void pack_s_sums_t(real *sum_send_t, int *offset, int *bin_start,
  int *bin_count, int *part_ind, int ncoeffs_max,
  real *int_Ys_re, real *int_Ys_im);

__global__ void pack_s_sums_b(real *sum_send_b, int *offset, int *bin_start,
  int *bin_count, int *part_ind, int ncoeffs_max,
  real *int_Ys_re, real *int_Ys_im);

__global__ void unpack_s_sums_e(real *sum_recv_e, int *offset, int *bin_start,
  int *bin_count, int *part_ind, int ncoeffs_max,
  real *int_Ys_re, real *int_Ys_im);

__global__ void unpack_s_sums_w(real *sum_recv_w, int *offset, int *bin_start,
  int *bin_count, int *part_ind, int ncoeffs_max,
  real *int_Ys_re, real *int_Ys_im);

__global__ void unpack_s_sums_n(real *sum_recv_n, int *offset, int *bin_start,
  int *bin_count, int *part_ind, int ncoeffs_max,
  real *int_Ys_re, real *int_Ys_im);

__global__ void unpack_s_sums_s(real *sum_recv_s, int *offset, int *bin_start,
  int *bin_count, int *part_ind, int ncoeffs_max,
  real *int_Ys_re, real *int_Ys_im);

__global__ void unpack_s_sums_t(real *sum_recv_t, int *offset, int *bin_start,
  int *bin_count, int *part_ind, int ncoeffs_max,
  real *int_Ys_re, real *int_Ys_im);

__global__ void unpack_s_sums_b(real *sum_recv_b, int *offset, int *bin_start,
  int *bin_count, int *part_ind, int ncoeffs_max,
  real *int_Ys_re, real *int_Ys_im);

__global__ void scalar_compute_coeffs(part_struct *parts,
  part_struct_scalar *s_parts, int s_ncoeffs_max, int nparts,
  real *int_Ys_re, real *int_Ys_im);

__global__ void scalar_part_BC(real *s, int *phase, int *phase_shell,
  part_struct *parts, part_struct_scalar *s_parts);

__global__ void scalar_compute_error(real lamb_cut_scalar, int s_ncoeffs_max, int nparts,
  part_struct_scalar *s_parts, real *s_part_errors);

__global__ void scalar_store_coeffs(part_struct_scalar *s_parts, int nparts,
  int s_ncoeffs_max);

__global__ void update_part_scalar(part_struct *parts,
  part_struct_scalar *s_parts, real time, real dt, real s_k);

__global__ void scalar_part_fill(real *s, int *phase,
  part_struct_scalar *s_parts);

#endif
