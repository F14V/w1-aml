
#ifdef RF_CALI_TEST
#include "rf_calibration.h"
void wf5g_sx(U8 channel, U8 osc_freq, SX_CALI_MODE_T sx_cali_mode)
{
    unsigned int unlock_cnt = 0;
    unsigned int TIME_OUT_CNT = 300;
    RG_SX_A6_FIELD_T    rg_sx_a6;
    RG_SX_A0_FIELD_T    rg_sx_a0;
    RG_SX_A2_FIELD_T    rg_sx_a2;

    // Internal variables definitions
    bool    RG_SX_A0_saved = False;
    RG_SX_A0_FIELD_T    rg_sx_a0_i;

    rg_sx_a6.data = rf_read_register(RG_SX_A6);
    rg_sx_a6.b.RG_SX_OSC_FREQ = osc_freq;
    rf_write_register(RG_SX_A6, rg_sx_a6.data);

    rg_sx_a6.data = rf_read_register(RG_SX_A6);
    rg_sx_a6.b.RG_5GB_SX_CHANNEL = channel;
    rf_write_register(RG_SX_A6, rg_sx_a6.data);

    rg_sx_a0.data = rf_read_register(RG_SX_A0);
    rg_sx_a0.b.RG_WF_SX_FCAL_MODE_SEL = sx_cali_mode;
    rf_write_register(RG_SX_A0, rg_sx_a0.data);

    rg_sx_a0.data = rf_read_register(RG_SX_A0);
    if(!RG_SX_A0_saved) {
        RG_SX_A0_saved = True;
        rg_sx_a0_i.b.RG_WF_SX_FCAL_EN = rg_sx_a0.b.RG_WF_SX_FCAL_EN;
    }
    rg_sx_a0.b.RG_WF_SX_FCAL_EN = 0x1;                         // Trigger SX
    rf_write_register(RG_SX_A0, rg_sx_a0.data);

    rg_sx_a2.data = rf_read_register(RG_SX_A2);
    while (unlock_cnt < TIME_OUT_CNT)
    {        
        if(rg_sx_a2.b.RO_CALI_END_FLG == 0)
        {     

            rg_sx_a2.data = rf_read_register(RG_SX_A2);
            g_fw_funcs.system_wait(1);
            unlock_cnt = unlock_cnt + 1;
            if(unlock_cnt == TIME_OUT_CNT)
            {
                PUTS("error: SX TIME OUT\n");
            } 
        }
        else
        {    
            PUTS("SX END\n");
            break;
        }      
    }
}

#endif //RF_CALI_TEST
