// Builder gateway function for BPSK Demodulation 

function builder_gw_cpp()
    WITHOUT_AUTO_PUTLHSVAR = %t;
    tbx_build_gateway("itpp_cpp", ..
    ["berc_get_errors","itpp_berc_get_errors";
	"bpsk_mod","itpp_bpsk_mod";
	"bpsk_demod","itpp_bpsk_demod";
	"crc_decode","itpp_crc_decode";
	"crc_encode","itpp_crc_encode";
	"hamm_decode","itpp_hamm_decode";
	"hamm_encode","itpp_hamm_encode";
	"psk_demod","itpp_psk_demod";
	"psk_mod","itpp_psk_mod";
	"qpsk_demod","itpp_qpsk_demod";
	"qpsk_mod","itpp_qpsk_mod";
	"rrc_get_pulse_shape","itpp_rrc_get_pulse_shape";
	"rrc_shape_symbols","itpp_rrc_shape_symbols";
	"seq_deinterleaver","itpp_seq_deinterleaver";
	"seq_interleaver","itpp_seq_interleaver";
	],..
	[
	"itpp_berc_get_errors.cpp",
	"itpp_bpsk_mod.cpp",
	"itpp_bpsk_demod.cpp",
	"itpp_crc_decode.cpp",
	"itpp_crc_encode.cpp",
	"itpp_hamm_decode.cpp",
	"itpp_hamm_encode.cpp",
	"itpp_psk_demod.cpp",
	"itpp_psk_mod.cpp",
	"itpp_qpsk_demod.cpp",
	"itpp_qpsk_mod.cpp",
	"itpp_rrc_get_pulse_shape.cpp",
	"itpp_rrc_shape_symbols.cpp",
	"itpp_seq_deinterleaver.cpp",
	"itpp_seq_interleaver.cpp",
	],..
    get_absolute_file_path("builder_gateway_cpp.sce"), [], "-litpp");

endfunction

builder_gw_cpp();
clear builder_gw_cpp; // remove builder_gw_cpp on stack
