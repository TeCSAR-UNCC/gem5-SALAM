#include "cacti_wrapper.hh"

uca_org_t cactiWrapper(unsigned num_of_bytes, unsigned wordsize, unsigned num_ports, int cache_type) {
  int cache_size = num_of_bytes;
  int line_size = wordsize;  // in bytes
  if (wordsize < 4)          // minimum line size in cacti is 32-bit/4-byte
    line_size = 4;
  if (cache_size / line_size < 64)
    cache_size = line_size * 64;  // minimum scratchpad size: 64 words
  int associativity = 1;
  int rw_ports = num_ports;
  if (rw_ports == 0)
    rw_ports = 1;
  int excl_read_ports = 0;
  int excl_write_ports = 0;
  int single_ended_read_ports = 0;
  int search_ports = 0;
  int banks = 1;
  double tech_node = 40;  // in nm
  //# following three parameters are meaningful only for main memories
  int page_sz = 0;
  int burst_length = 8;
  int pre_width = 8;
  int output_width = wordsize * 8;
  //# to model special structure like branch target buffers, directory, etc.
  //# change the tag size parameter
  //# if you want cacti to calculate the tagbits, set the tag size to "default"
  int specific_tag = false;
  int tag_width = 0;
  int access_mode = 2;  // 0 normal, 1 seq, 2 fast
  int cache = cache_type;   // scratch ram 0 or cache 1
  int main_mem = 0;
  // assign weights for CACTI optimizations
  int obj_func_delay = 0;
  int obj_func_dynamic_power = 0;
  int obj_func_leakage_power = 100;
  int obj_func_area = 0;
  int obj_func_cycle_time = 0;
  // from CACTI example config...
  int dev_func_delay = 20;
  int dev_func_dynamic_power = 100000;
  int dev_func_leakage_power = 100000;
  int dev_func_area = 1000000;
  int dev_func_cycle_time = 1000000;

  int ed_ed2_none = 2;  // 0 - ED, 1 - ED^2, 2 - use weight and deviate
  int temp = 300;
  int wt = 0;  // 0 - default(search across everything), 1 - global, 2 - 5%
               // delay penalty, 3 - 10%, 4 - 20 %, 5 - 30%, 6 - low-swing
  int data_arr_ram_cell_tech_flavor_in =
      0;  // 0(itrs-hp) 1-itrs-lstp(low standby power)
  int data_arr_peri_global_tech_flavor_in = 0;  // 0(itrs-hp)
  int tag_arr_ram_cell_tech_flavor_in = 0;      // itrs-hp
  int tag_arr_peri_global_tech_flavor_in = 0;   // itrs-hp
  int interconnect_projection_type_in = 1;      // 0 - aggressive, 1 - normal
  int wire_inside_mat_type_in = 1;   // 2 - global, 0 - local, 1 - semi-global
  int wire_outside_mat_type_in = 1;  // 2 - global
  int REPEATERS_IN_HTREE_SEGMENTS_in =
      1;  // TODO for now only wires with repeaters are supported
  int VERTICAL_HTREE_WIRES_OVER_THE_ARRAY_in = 0;
  int BROADCAST_ADDR_DATAIN_OVER_VERTICAL_HTREES_in = 0;
  int force_wiretype = 1;
  int wiretype = 30;
  int force_config = 0;
  int ndwl = 1;
  int ndbl = 1;
  int nspd = 0;
  int ndcm = 1;
  int ndsam1 = 0;
  int ndsam2 = 0;
  int ecc = 0;
  return cacti_interface(cache_size,
                         line_size,
                         associativity,
                         rw_ports,
                         excl_read_ports,
                         excl_write_ports,
                         single_ended_read_ports,
                         search_ports,
                         banks,
                         tech_node,  // in nm
                         output_width,
                         specific_tag,
                         tag_width,
                         access_mode,  // 0 normal, 1 seq, 2 fast
                         cache,        // scratch ram or cache
                         main_mem,
                         obj_func_delay,
                         obj_func_dynamic_power,
                         obj_func_leakage_power,
                         obj_func_cycle_time,
                         obj_func_area,
                         dev_func_delay,
                         dev_func_dynamic_power,
                         dev_func_leakage_power,
                         dev_func_area,
                         dev_func_cycle_time,
                         ed_ed2_none,
                         temp,
                         wt,
                         data_arr_ram_cell_tech_flavor_in,
                         data_arr_peri_global_tech_flavor_in,
                         tag_arr_ram_cell_tech_flavor_in,
                         tag_arr_peri_global_tech_flavor_in,
                         interconnect_projection_type_in,
                         wire_inside_mat_type_in,
                         wire_outside_mat_type_in,
                         REPEATERS_IN_HTREE_SEGMENTS_in,
                         VERTICAL_HTREE_WIRES_OVER_THE_ARRAY_in,
                         BROADCAST_ADDR_DATAIN_OVER_VERTICAL_HTREES_in,
                         page_sz,
                         burst_length,
                         pre_width,
                         force_wiretype,
                         wiretype,
                         force_config,
                         ndwl,
                         ndbl,
                         nspd,
                         ndcm,
                         ndsam1,
                         ndsam2,
                         ecc);
}