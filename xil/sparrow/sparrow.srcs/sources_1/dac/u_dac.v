`include "sparrow_mmap.v"

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

///input [9:0]i_dat;
output o_dac_clk_p;
output o_dac_clk_m;
///output o_dac_align_p;
///output o_dac_align_m;
///output o_dac_sync_p;
///output o_dac_sync_m;
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
////wire o_dac_align_p;
////wire o_dac_align_m;
////wire o_dac_sync_p;
////wire o_dac_sync_m;
wire o_dac_dclk_p;
wire o_dac_dclk_m;
wire [6:0]o_dac_dat_p;
wire [6:0]o_dac_dat_m;
///=====================================
wire sregs_we_dat;
assign sregs_we_dat=i_cs&(i_addr[13:12]==`ADDR_DAC_REGS)&(i_addr[15:14]==`ADDR_COMMON)&i_we;
reg [15:0]conf;
///wire ena_dac_clk67;
///wire sync_out;
////wire allign_out;

wire ena_tst_dac_out;
assign ena_tst_dac_out=conf[0];
///assign ena_dac_clk67=conf[1];
///assign sync_out=conf[2];
////assign allign_out=conf[3];


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

///====================================
reg [13:0]tst_cnt;
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	tst_cnt = 14'b0;				///
else
	tst_cnt =tst_cnt+1'b1; 
///====================================
assign tst= tst_cnt[3:0];
wire [13:0]tt_odat;
wire [13:0]t_odat;

assign tt_odat= (ena_tst_dac_out)?dac_odata[13:0]:tst_cnt[13:0];
assign t_odat= tt_odat;
///====================================
genvar ii;
/*
generate
for (ii=0;ii<7;ii=ii+1)
begin: dac_data
 OBUFDS 
  #(.IOSTANDARD("LVDS_25")
///        ,.SLEW("SLOW")
        )
dac_data_obuf_ii (
    .I (t_odat[ii]),
    .O (o_dac_dat_p[ii]),
    .OB (o_dac_dat_n[ii]));
 end
 endgenerate   
*/
generate
for (ii=0;ii<7;ii=ii+1)
begin: dac_data
dac_data_out 
dac_data_( 
 .tx_clk(i_clk)
/// ,.tx_data_p(t_odat[ii+7])
 ///, .tx_data_m(t_odat[ii])
 ,.tx_data_p(t_odat[ii])
 , .tx_data_m(t_odat[ii+7])
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
    
 ///wire t_dac_sync;
 ///assign  t_dac_sync=(ena_dac_clk67)?i_clk:sync_out;  
 /* 
 OBUFDS 
  #(.IOSTANDARD("LVDS_25")
///        ,.SLEW("SLOW")
        )
dac_sync_obuf (
    .I (t_dac_sync),
    .O (o_dac_sync_p),
    .OB (o_dac_sync_m));
 */   
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
////assign ena_dac_clk67=conf[1];
///assign sync_out=conf[2];
/*
 wire t_dac_align;
 assign  t_dac_align=allign_out;  
 OBUFDS 
  #(.IOSTANDARD("LVDS_25")
///        ,.SLEW("SLOW")
        )
dac_align_obuf (
    .I (t_dac_align),
    .O (o_dac_align_p),
    .OB (o_dac_align_m));
    
*/
/*
dac_data_out 
dac_align_( 
 .tx_clk(i_clk)
 ,.tx_data_p(t_dac_align)
 , .tx_data_n(1'b0)
 ,.tx_data_out_p(o_dac_align_p)
 ,.tx_data_out_n(o_dac_align_n)
);
*/
///====================================
always @*
begin
case(i_addr[15:14])         ///1
	`ADDR_COMMON:
	   case(i_addr[13:12])     ///2
	   `ADDR_DAC_REGS:
	   		case(i_addr[6:1])     ///3
			`OFFS_CONF:
				o_data <= conf;
			`OFFS_DAC_OUT:
				o_data <= dac_odata;
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
endmodule

