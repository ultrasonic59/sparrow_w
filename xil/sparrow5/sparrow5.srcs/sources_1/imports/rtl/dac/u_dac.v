`include "sparrow5_mmap.v"

module u_dac(i_ps_clk
            ,i_clk,i_clr
            ,i_cs,i_we,i_addr
			,i_data,o_data
            ,o_dac_clk_p,o_dac_clk_m
////           ,o_dac_align_p,o_dac_align_m
////           ,o_dac_sync_p,o_dac_sync_m
           ,o_dac_dclk_p,o_dac_dclk_m
           ,o_dac_dat_p,o_dac_dat_m
           ,i_sync
		    ,tst);
///====================================
input i_ps_clk;
input i_clk,i_clr;
input i_cs;
input i_we;
input [15:0]i_addr;
input [15:0]i_data;
output [15:0]o_data;

output o_dac_clk_p;
output o_dac_clk_m;
output o_dac_dclk_p;
output o_dac_dclk_m;
output [6:0]o_dac_dat_p;
output [6:0]o_dac_dat_m;
input i_sync;
///====================================
///====================================
///===== tst ==================
output [3:0]tst;
///====================================
reg [15:0]o_data;
reg [15:0]dac_odata;

wire [3:0]tst;

wire o_dac_clk_p;
wire o_dac_clk_m;
wire o_dac_dclk_p;
wire o_dac_dclk_m;
wire [6:0]o_dac_dat_p;
wire [6:0]o_dac_dat_m;
///=====================================
reg [3:0]z_sync;

wire sregs_we_dat;
assign sregs_we_dat=i_cs&(i_addr[13:12]==`ADDR_DAC_REGS)&(i_addr[15:14]==`ADDR_COMMON)&i_we;
reg [15:0]conf;
reg [31:0]dds_val;
reg [31:0]dds_b_ph;///beg phasa
reg [15:0]len_cnt_dac;
reg [15:0]delay_cnt_dac;
reg [15:0]period_us;

wire ena_tst_dac_out;
assign ena_tst_dac_out=conf[`B_ON_TST];
wire on_dds;
assign on_dds=conf[`B_ON_DDS];
wire en_dds;
assign en_dds=conf[`B_EN_DDS];
wire dds_ph_dat;
assign dds_ph_dat=conf[`B_DDS_PH_DAT];


always @(negedge sregs_we_dat )
begin
if(i_addr[6:1] ==`OFFS_CONF)
   conf= i_data[15:0];
end
always @(negedge sregs_we_dat )
begin
if(i_addr[6:1] ==`OFFS_DAC_OUT)
   dac_odata= i_data[15:0];
end
always @(negedge sregs_we_dat )
begin
if(i_addr[6:1] ==`OFFS_B_PH_L)
   dds_b_ph[15:0]= i_data[15:0];
end

always @(negedge sregs_we_dat )
begin
if(i_addr[6:1] ==`OFFS_B_PH_H)
   dds_b_ph[31:16]= i_data[15:0];
end
always @(negedge sregs_we_dat )
begin
if(i_addr[6:1] ==`OFFS_DDS_L)
   dds_val[15:0]<= i_data[15:0];
end

always @(negedge sregs_we_dat )
begin
if(i_addr[6:1] ==`OFFS_DDS_H)
   dds_val[31:16]<= i_data[15:0];
end
always @(negedge sregs_we_dat )
begin
if(i_addr[6:1] ==`OFFS_LEN_CNT_DAC)
   len_cnt_dac[15:0]<= i_data[15:0];
end
always @(negedge sregs_we_dat )
begin
if(i_addr[6:1] ==`OFFS_DELAY_CNT_DAC)
   delay_cnt_dac[15:0]<= i_data[15:0];
end
always @(negedge sregs_we_dat )
begin
if(i_addr[6:1] == 6'h08)
   period_us[15:0]<= i_data[15:0];
end

///====================================
reg [15:0]cnt_delay;
wire end_delay_cnt_dac;
assign end_delay_cnt_dac=(cnt_delay>=delay_cnt_dac);

/// One sync_in edge, then psk_dac0 and one clock later psk_dac1. A wide
/// or repeated sync_in cannot produce a second pair.
reg [1:0] start_ph;
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	start_ph <= 2'b0;
else if(start_ph != 2'b0)
	start_ph <= (start_ph == 2'd2) ? 2'b0 : start_ph + 2'b1;
else if(sync_in)
	start_ph <= 2'd1;
wire psk_dac0 = (start_ph == 2'd1);
wire psk_dac1 = (start_ph == 2'd2);

reg trig_pend;
wire go = trig_pend & end_delay_cnt_dac & !psk_dac1;
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	trig_pend <= 1'b0;
else if(psk_dac0)
	trig_pend <= 1'b1;
else if(go)
	trig_pend <= 1'b0;

reg [12:0]dac_cnt;
wire end_cnt_dac;
assign end_cnt_dac=(dac_cnt>=len_cnt_dac[12:0]);

reg ena_cnt_dac;
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	ena_cnt_dac <= 1'b0;				///
else if(psk_dac1)
	ena_cnt_dac <= 1'b0;				///
else if(end_cnt_dac)
	ena_cnt_dac <=1'b0; 
else if(go)
	ena_cnt_dac <=1'b1; 
reg ena_cnt_delay;
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	ena_cnt_delay <= 1'b0;				///
else if(end_delay_cnt_dac)
	ena_cnt_delay <=1'b0; 
else if(psk_dac0)
	ena_cnt_delay <= 1'b1;				///
	
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	cnt_delay <= 16'b0;				///
else if(psk_dac0)
	cnt_delay <= 16'b0;				///
else if(ena_cnt_delay)
	cnt_delay <=cnt_delay+1'b1; 

/// 250 MHz DAC clock. One model sample lasts 2 clocks: 125 MSPS.
reg half;
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	dac_cnt <= 13'b0;
else if(psk_dac1)
	dac_cnt <= 13'b0;
else if(ena_cnt_dac && half)
	dac_cnt <= dac_cnt + 1'b1;
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	half <= 1'b0;
else if(psk_dac1)
	half <= 1'b0;
else if(ena_cnt_dac)
	half <= ~half;

/// Replay the buffer: after playback ends, wait period_us, then the same edge as i_sync.
reg [8:0]gap_tick;
reg [15:0]gap_us;
reg freerun_arm;
reg freerun_pulse;
wire freerun_on = conf[6];
wire play_idle = !ena_cnt_dac && !ena_cnt_delay;
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	begin
	gap_tick <= 9'b0;
	gap_us <= 16'b0;
	freerun_arm <= 1'b1;
	freerun_pulse <= 1'b0;
	end
else if(!freerun_on)
	begin
	gap_tick <= 9'b0;
	gap_us <= 16'b0;
	freerun_arm <= 1'b1;
	freerun_pulse <= 1'b0;
	end
else if(!play_idle)
	begin
	gap_tick <= 9'b0;
	gap_us <= 16'b0;
	freerun_arm <= 1'b1;
	freerun_pulse <= 1'b0;
	end
else if(freerun_pulse)
	begin
	freerun_pulse <= 1'b0;
	freerun_arm <= 1'b0;
	gap_tick <= 9'b0;
	gap_us <= 16'b0;
	end
else if(freerun_arm && ((period_us == 16'b0) || (gap_us >= period_us)))
	freerun_pulse <= 1'b1;
else if(freerun_arm && (gap_tick == 9'd249))
	begin
	gap_tick <= 9'b0;
	gap_us <= gap_us + 16'b1;
	end
else if(freerun_arm)
	gap_tick <= gap_tick + 9'b1;

/// Two flops move the comparator input into the DAC clock domain.
/// One rising edge produces one start. Further edges are ignored for 2 us.
(* ASYNC_REG = "TRUE" *) reg pin_meta;
(* ASYNC_REG = "TRUE" *) reg pin_sync;
reg pin_d;
reg [2:0]arm_ff;
reg [1:0]once_ff;
reg [1:0]fal_ff;
reg ext_en;
reg [31:0]trig_cnt;
reg [31:0]trig_ps;
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	begin
	pin_meta <= 1'b0;
	pin_sync <= 1'b0;
	pin_d <= 1'b0;
	arm_ff <= 3'b0;
	once_ff <= 2'b0;
	fal_ff <= 2'b0;
	end
else
	begin
	pin_meta <= i_sync;
	pin_sync <= pin_meta;
	pin_d <= pin_sync;
	arm_ff <= {arm_ff[1:0], conf[`B_EXT_SYNC]};
	once_ff <= {once_ff[0], conf[7]};
	fal_ff <= {fal_ff[0], conf[`B_EXT_FAL]};
	end
wire pin_rise = pin_sync & ~pin_d;
wire pin_fall = ~pin_sync & pin_d;
wire pin_edge = fal_ff[1] ? pin_fall : pin_rise;
/// 250 MHz DAC clock: 500 ticks is 2 us.
localparam [9:0] LOCK_TICKS = 10'd500;
reg [9:0] lock_cnt;
wire locked = (lock_cnt != 10'd0);
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	lock_cnt <= 10'd0;
else if(ext_en && pin_edge && !locked)
	lock_cnt <= LOCK_TICKS;
else if(locked)
	lock_cnt <= lock_cnt - 10'd1;
wire ext_pulse = ext_en & pin_edge & !locked;
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	begin
	ext_en <= 1'b0;
	trig_cnt <= 32'b0;
	end
else if(!arm_ff[1])
	ext_en <= 1'b0;
else if(arm_ff[1] && !arm_ff[2])
	begin
	ext_en <= 1'b1;
	trig_cnt <= 32'b0;
	end
else if(go && once_ff[1])
	ext_en <= 1'b0;
else if(ext_pulse)
	trig_cnt <= trig_cnt + 32'b1;
always @(posedge i_ps_clk)
	trig_ps <= trig_cnt;
reg ext_pulse_r;
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	ext_pulse_r <= 1'b0;
else
	ext_pulse_r <= ext_pulse;
wire sync_in = ext_pulse_r | freerun_pulse;
///====================================
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	z_sync <= 4'b0;				///
else
	z_sync <={z_sync[2:0],sync_in}; 

wire [13:0]tt_odat;
wire [13:0]t_odat;
wire [13:0]dds_out;
wire [31:0]curr_ph;

wire ps_we_ram;
wire [15:0]ps_ram_odata;
wire [12:0]dac_ram_addr;
wire [15:0]dac_ram_odata;
wire [13:0]_dac_ram_odata;
/// Capture the BRAM word first. Its block mux is combinational, and sampling
/// it in the same cycle as the offset-binary conversion latches a short spike
/// each time the phase crosses a 2K boundary, at any tone frequency.
reg signed [15:0]samp_r;
always @(posedge i_clk)
	samp_r <= dac_ram_odata;
wire signed [16:0]samp_ext = {samp_r[15], samp_r};
wire signed [16:0]code_s = 17'sd8192 - (samp_ext >>> 2);
wire [13:0]play_code = (code_s < 0) ? 14'h0 : ((code_s > 17'sd16383) ? 14'h3FFF : code_s[13:0]);
assign _dac_ram_odata=(ena_cnt_dac)?play_code:14'h2000;
assign t_odat= (ena_tst_dac_out)?dac_odata[13:0]
                :(on_dds)?(dds_ph_dat? {dac_ram_addr,1'b0}:dds_out)
                :dds_ph_dat? {dac_cnt,1'b0}:_dac_ram_odata;
reg [13:0]t_odat_r;
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	t_odat_r <= 14'h2000;
else
	t_odat_r <= t_odat;
assign tt_odat= t_odat_r;
///====================================

/// Sine table in RAM is signed int16. DDS uses the same offset-binary coding as playback.
assign dds_out = play_code;
assign dac_ram_addr=(on_dds)?curr_ph[31:19]:dac_cnt[12:0];

assign ps_we_ram=i_cs&(i_addr[15:14]==`ADDR_DAC_RAM)&i_we;
mem_2kx16 mem_dac(
    .clka(i_ps_clk)
    ,.ena(1'b1)
    ,.wea(ps_we_ram)
    ,.addra(i_addr[13:1])
    ,.dina(i_data)
    ,.douta(ps_ram_odata)
    ,.clkb(i_clk)
    ,.enb(on_dds | ena_cnt_dac)
    ,.web(1'b0)
    ,.addrb(dac_ram_addr)
    ,.dinb(16'b0)
    ,.doutb(dac_ram_odata)
  );

///====================================
phase_acc ph_acc(.clk_i(i_clk),
                .clr_i(i_clr),
                .phase_en_i(en_dds),
                .sync_i(1'b0),
                .b_phase_i(dds_b_ph),
                .dds_val_i(dds_val),
                .phase_o(curr_ph)
                );
///====================================

///===============================
ila_0 ila0(
 .clk(i_clk),
 .probe0(conf),
 .probe1( z_sync[2:0]),
 .probe2(cnt_delay), 
 .probe3(dac_cnt),
 .probe4(dac_ram_addr),
 .probe5(dac_ram_odata),
 ///.probe4(curr_ph[31:18]),
 .probe6(tt_odat),
 .probe7(ena_cnt_dac),
  .probe8(ena_cnt_delay)
);
///===================================


///====================================

genvar ii;
generate
for (ii=0;ii<7;ii=ii+1)
begin: dac_data
dac_data_out 
dac_data_( 
 .tx_clk(i_clk)
 ,.tx_data_p(t_odat_r[ii])
 , .tx_data_m(t_odat_r[ii+7])
 ,.tx_data_out_p(o_dac_dat_p[ii])
 ,.tx_data_out_m(o_dac_dat_m[ii])
);
 end
 endgenerate   

///============================================================    
 OBUFDS 
 #(.IOSTANDARD("LVDS_25")
///        ,.SLEW("SLOW")
        )
 dac_clk_obuf (
    .I (i_clk),
    .O (o_dac_clk_p),
    .OB (o_dac_clk_m));
    
 wire t_dac_dclk;
 assign  t_dac_dclk=i_clk;  
 OBUFDS 
  #(.IOSTANDARD("LVDS_25")
///        ,.SLEW("SLOW")
        )
dac_dclk_obuf (
    .I (t_dac_dclk),
    .O (o_dac_dclk_p),
    .OB (o_dac_dclk_m));

///====================================
///====================================
always @*
begin
case(i_addr[15:14])         ///1
    `ADDR_DAC_RAM:
        o_data <= ps_ram_odata;
	`ADDR_COMMON:
	   case(i_addr[13:12])     ///2
	   `ADDR_DAC_REGS:
	   		case(i_addr[6:1])     ///3
			`OFFS_CONF:
				o_data <= conf;
			`OFFS_DAC_OUT:
				o_data <= dac_odata;
            `OFFS_B_PH_L:
                o_data <= dds_b_ph[15:0];
            `OFFS_B_PH_H:
                o_data <= dds_b_ph[31:16];
           `OFFS_DDS_L:
 				o_data <= dds_val[15:0];
           `OFFS_DDS_H:
				o_data <= dds_val[31:16];
            `OFFS_LEN_CNT_DAC:
                o_data <= len_cnt_dac;
            `OFFS_DELAY_CNT_DAC:
                 o_data <= delay_cnt_dac;
            6'h08:
                 o_data <= period_us;
            6'h09:
                 o_data <= trig_ps[15:0];
            6'h0A:
                 o_data <= trig_ps[31:16];
              default:    
	           o_data <= 16'h3333;
          endcase         ///3
        default:    
	       o_data <= 16'h2222;
	    endcase        ///2
    default:    
      o_data <= 16'h1111;
    endcase        ///1
end   
///=============================================
assign tst[0]= ena_cnt_dac;
assign tst[1]= ena_cnt_delay;
assign tst[2]= psk_dac1;
/// o_zz[5]: high for the 2 us lockout that begins at the accepted edge.
assign tst[3]= locked;


endmodule

