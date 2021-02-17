`include "sparrow_mmap.v"

module u_adc #(
    parameter   IODELAY_GROUP = "dev_if_delay_group",
    parameter   REFCLK_FREQUENCY = 200)
(i_ps_clk,i_clk,i_clr
            ,i_delay_clk
            ,i_cs,i_we,i_addr
			,i_data,o_data
            ,i_adc_clk_p,i_adc_clk_n
            ,o_adc_clk_p,o_adc_clk_n
            ,i_data_p,i_data_n
           ,i_sync
  		    ,tst);
///====================================
input i_ps_clk;
input i_clk,i_clr;
input i_delay_clk;
input i_cs;
input i_we;
input [15:0]i_addr;
input [15:0]i_data;
output [15:0]o_data;
///========================
input i_adc_clk_p;
input i_adc_clk_n;
input [6:0]i_data_p;
input [6:0]i_data_n;
output o_adc_clk_p;
output o_adc_clk_n;

input i_sync;
///====================================
///====================================
///===== tst ==================
output [3:0]tst;
///====================================
wire o_adc_clk_p;
wire o_adc_clk_n;

reg delay_rst;
wire delay_locked;
reg [15:0]o_data;

///====================================================
wire sregs_we_dat;
assign sregs_we_dat=i_cs&(i_addr[13:12]==`ADDR_ADC_REGS)&(i_addr[15:14]==`ADDR_COMMON)&i_we;
reg [15:0]conf;
reg [34:0]delay_wdat;
wire [34:0]delay_rdat;    
reg [6:0]up_dld;

always @(negedge sregs_we_dat )
begin
if(i_addr[6:1] ==`OFFS_CONF)
   conf<= i_data[15:0];
end
reg [6:0]t_up_dld;

reg [6:0]clr_dld;

generate
genvar jj;
for (jj = 0; jj < 7; jj = jj + 1) 
begin : gen_set_delay_data
always @(posedge i_delay_clk )
if(i_clr)
    t_up_dld[jj]<=1'b0;
else if(clr_dld[jj])
    t_up_dld[jj]<=1'b0;
else if((i_addr[6:1] == `OFFS_DELAY_WDAT0+jj)&sregs_we_dat)
    begin
   delay_wdat[((jj*5)+4):(jj*5)]= i_data[4:0];
   t_up_dld[jj]<=1'b1;
   end
end
endgenerate
generate
for (jj = 0; jj < 7; jj = jj + 1) begin : gen_set_dld
always @(posedge i_delay_clk )
   up_dld[jj]<= t_up_dld[jj];
end
endgenerate
always @(posedge i_delay_clk )
if(i_clr)
  clr_dld<=7'b0;
else  
   clr_dld<= up_dld;
///====================================================
reg [ 3:0]idelay_reset_cnt;

always @(posedge i_delay_clk) begin
 if (i_clr == 1'b1) begin
        idelay_reset_cnt <= 4'h0;
        delay_rst <= 1'b1;
      end else begin
        delay_rst <= 1'b1;
        case (idelay_reset_cnt)
          4'h0: idelay_reset_cnt <= 4'h1;
          4'h1: idelay_reset_cnt <= 4'h2;
          4'h2: idelay_reset_cnt <= 4'h3;
          4'h3: idelay_reset_cnt <= 4'h4;
          4'h4: idelay_reset_cnt <= 4'h5;
          4'h5: idelay_reset_cnt <= 4'h6;
          4'h6: idelay_reset_cnt <= 4'h7;
          4'h7: idelay_reset_cnt <= 4'h8;
          4'h8: idelay_reset_cnt <= 4'h9;
          4'h9: idelay_reset_cnt <= 4'ha;
          4'ha: idelay_reset_cnt <= 4'hb;
          4'hb: idelay_reset_cnt <= 4'hc;
          4'hc: idelay_reset_cnt <= 4'hd;
          4'hd: idelay_reset_cnt <= 4'he;
          default: begin
            idelay_reset_cnt <= 4'he;
            delay_rst <= 1'b0;
          end
   endcase
end
end

///====================================================
(* IODELAY_GROUP = IODELAY_GROUP *)
  IDELAYCTRL #(.SIM_DEVICE ("7SERIES")) i_delay_ctrl (
    .RST (delay_rst),
    .REFCLK (i_delay_clk),
    .RDY (delay_locked));

///====================================
reg [13:0]tst_cnt;
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	tst_cnt = 14'b0;				///
else
	tst_cnt =tst_cnt+1'b1; 
///====================================

wire [6:0]t_odat;
assign t_odat=tst_cnt[6:0];
///====================================
/*
 OBUFDS dac_data_obuf (
    .I (t_odat),
    .O (o_dac_dat_p),
    .OB (o_dac_dat_n));
*/    
    
 OBUFDS #(.IOSTANDARD("LVDS_25")
        ,.SLEW("SLOW"))
 adc_clk_obuf (
    .I (i_clk),
    .O (o_adc_clk_p),
    .OB (o_adc_clk_n));
    
wire [13:0]t_data;
wire up_clk;
assign up_clk=i_ps_clk;

 generate
 genvar ii;
 for (ii = 0; ii < 7; ii = ii + 1) 
 begin : gen_adc_data
 ad_data_in #(
    .IODELAY_GROUP (IODELAY_GROUP),
    .REFCLK_FREQUENCY (REFCLK_FREQUENCY))
  i_adc_data (
    .i_clk (i_clk),
    .i_data_p (i_data_p[ii]),
    .i_data_n (i_data_n[ii]),
    .o_data (t_data[((ii*2)+1):(ii*2)]),
 ///   .o_data (t_data[ii]),
    .up_clk (up_clk),
    .up_dld (up_dld[ii]),
   .up_dwdata (delay_wdat[((ii*5)+4):(ii*5)]),
    .up_drdata (delay_rdat[((ii*5)+4):(ii*5)]),
    .delay_clk (i_delay_clk),
    .delay_rst (delay_rst),
    .delay_locked ());
end
endgenerate
wire [13:0]_t_data;
assign _t_data=14'h32a5;
///==================================
///===============================
ila_0 ila0(
 .clk(i_clk),
  .probe0(delay_wdat),
 .probe1(delay_rdat),
 .probe2(t_data)
 );
///===================================



always @*
begin
case(i_addr[15:14])         ///1
	`ADDR_COMMON:
	   case(i_addr[13:12])     ///2
	   `ADDR_ADC_REGS:
	   		case(i_addr[6:1])     ///3
			`OFFS_ADC_CONF:
				o_data <= conf;
			`OFFS_ADC_OUT:
				o_data <= {2'b0,t_data};
			`OFFS_ADC_CHECK:
				o_data <= 16'ha3a3;
			`OFFS_DELAY_WDAT0:
				o_data <= {11'b0,delay_wdat[4:0]};
			`OFFS_DELAY_WDAT1:
				o_data <= {11'b0,delay_wdat[9:5]};
			`OFFS_DELAY_WDAT2:
				o_data <= {11'b0,delay_wdat[14:10]};
			`OFFS_DELAY_WDAT3:
				o_data <= {11'b0,delay_wdat[19:15]};
			`OFFS_DELAY_WDAT4:
				o_data <= {11'b0,delay_wdat[24:20]};
			`OFFS_DELAY_WDAT5:
				o_data <= {11'b0,delay_wdat[29:25]};
			`OFFS_DELAY_WDAT6:
				o_data <= {11'b0,delay_wdat[34:30]};
			`OFFS_DELAY_RDAT0:
				o_data <= {11'b0,delay_rdat[4:0]};
			`OFFS_DELAY_RDAT1:
				o_data <= {11'b0,delay_rdat[9:5]};
			`OFFS_DELAY_RDAT2:
				o_data <= {11'b0,delay_rdat[14:10]};
			`OFFS_DELAY_RDAT3:
				o_data <= {11'b0,delay_rdat[19:15]};
			`OFFS_DELAY_RDAT4:
				o_data <= {11'b0,delay_rdat[24:20]};
			`OFFS_DELAY_RDAT5:
				o_data <= {11'b0,delay_rdat[29:25]};
			`OFFS_DELAY_RDAT6:
				o_data <= {11'b0,delay_rdat[34:30]};
             default:    
	           o_data <= 16'h3333;
          endcase         ///3
        default:    
	       o_data <= 16'h2244;
	    endcase        ///2
    default:    
      o_data <= 16'h1155;
    endcase        ///1
end   
   
 ///====================================
endmodule

