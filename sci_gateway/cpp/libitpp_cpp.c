#ifdef __cplusplus
extern "C" {
#endif
#include <mex.h> 
#include <sci_gateway.h>
#include <api_scilab.h>
#include <MALLOC.h>
static int direct_gateway(char *fname,void F(void)) { F();return 0;};
extern Gatefunc itpp_berc_get_errors;
extern Gatefunc sci_berc_get_errorrate;
extern Gatefunc itpp_bpsk_mod;
extern Gatefunc itpp_bpsk_demod;
extern Gatefunc itpp_crc_decode;
extern Gatefunc itpp_crc_encode;
extern Gatefunc itpp_hamm_decode;
extern Gatefunc itpp_hamm_encode;
extern Gatefunc itpp_psk_demod;
extern Gatefunc itpp_psk_mod;
extern Gatefunc itpp_qpsk_demod;
extern Gatefunc itpp_qpsk_mod;
extern Gatefunc itpp_rrc_get_pulse_shape;
extern Gatefunc itpp_rrc_shape_symbols;
extern Gatefunc itpp_seq_deinterleaver;
extern Gatefunc itpp_seq_interleaver;
extern Gatefunc itpp_block_interleaver;
extern Gatefunc itpp_cross_deinterleaver;
extern Gatefunc itpp_cross_interleaver;
extern Gatefunc sci_cpp_ofdm_demod;
extern Gatefunc sci_cpp_ofdm_mod;
extern Gatefunc sci_cpp_pam_demod;
extern Gatefunc sci_cpp_pam_mod;
extern Gatefunc sci_cpp_qam_demod;
extern Gatefunc sci_cpp_qam_mod;
extern Gatefunc rc_get_pulse_shape;
extern Gatefunc rc_set_pulse_shape;
extern Gatefunc rc_shape_symbols;
extern Gatefunc sci_bch_dec;
extern Gatefunc sci_bch_enc;
extern Gatefunc sci_conv_dec;
extern Gatefunc sci_conv_enc;
extern Gatefunc sci_despread;
extern Gatefunc sci_spread;
extern Gatefunc sci_rsdec;
extern Gatefunc sci_rsenc;
static GenericTable Tab[]={
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_berc_get_errors,"berc_get_errors"},
  {(Myinterfun)sci_gateway_without_putlhsvar,sci_berc_get_errorrate,"berc_get_errorrate"},
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_bpsk_mod,"bpsk_mod"},
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_bpsk_demod,"bpsk_demod"},
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_crc_decode,"crc_decode"},
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_crc_encode,"crc_encode"},
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_hamm_decode,"hamm_decode"},
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_hamm_encode,"hamm_encode"},
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_psk_demod,"psk_demod"},
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_psk_mod,"psk_mod"},
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_qpsk_demod,"qpsk_demod"},
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_qpsk_mod,"qpsk_mod"},
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_rrc_get_pulse_shape,"rrc_get_pulse_shape"},
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_rrc_shape_symbols,"rrc_shape_symbols"},
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_seq_deinterleaver,"seq_deinterleaver"},
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_seq_interleaver,"seq_interleaver"},
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_block_interleaver,"block_interleaver"},
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_cross_deinterleaver,"cross_deinterleaver"},
  {(Myinterfun)sci_gateway_without_putlhsvar,itpp_cross_interleaver,"cross_interleaver"},
  {(Myinterfun)sci_gateway_without_putlhsvar,sci_cpp_ofdm_demod,"ofdm_demod"},
  {(Myinterfun)sci_gateway_without_putlhsvar,sci_cpp_ofdm_mod,"ofdm_mod"},
  {(Myinterfun)sci_gateway_without_putlhsvar,sci_cpp_pam_demod,"pam_demod"},
  {(Myinterfun)sci_gateway_without_putlhsvar,sci_cpp_pam_mod,"pam_mod"},
  {(Myinterfun)sci_gateway_without_putlhsvar,sci_cpp_qam_demod,"qam_demod"},
  {(Myinterfun)sci_gateway_without_putlhsvar,sci_cpp_qam_mod,"qam_mod"},
  {(Myinterfun)sci_gateway_without_putlhsvar,rc_get_pulse_shape,"rc_get_pulse_shape"},
  {(Myinterfun)sci_gateway_without_putlhsvar,rc_set_pulse_shape,"rc_set_pulse_shape"},
  {(Myinterfun)sci_gateway_without_putlhsvar,rc_shape_symbols,"rc_shape_symbols"},
  {(Myinterfun)sci_gateway_without_putlhsvar,sci_bch_dec,"bch_dec"},
  {(Myinterfun)sci_gateway_without_putlhsvar,sci_bch_enc,"bch_enc"},
  {(Myinterfun)sci_gateway_without_putlhsvar,sci_conv_dec,"conv_dec"},
  {(Myinterfun)sci_gateway_without_putlhsvar,sci_conv_enc,"conv_enc"},
  {(Myinterfun)sci_gateway_without_putlhsvar,sci_despread,"despread"},
  {(Myinterfun)sci_gateway_without_putlhsvar,sci_spread,"spread"},
  {(Myinterfun)sci_gateway_without_putlhsvar,sci_rsdec,"rsdec"},
  {(Myinterfun)sci_gateway_without_putlhsvar,sci_rsenc,"rsenc"},
};
 
int C2F(libitpp_cpp)()
{
  Rhs = Max(0, Rhs);
  if (*(Tab[Fin-1].f) != NULL) 
  {
     if(pvApiCtx == NULL)
     {
       pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
     }
     pvApiCtx->pstName = (char*)Tab[Fin-1].name;
    (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  }
  return 0;
}
#ifdef __cplusplus
}
#endif
