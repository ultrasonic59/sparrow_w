`include "sparrow_mmap.v"

module u_dac(i_ps_clk
            ,i_clk,i_clr
            ,i_cs,i_we,i_addr
			,i_data,o_data
            ,o_dac_clk_p,o_dac_clk_m
           ,o_dac_dclk_p,o_dac_dclk_m
           ,o_dac_dat_p,o_dac_dat_m
           ,i_sync
           ,dac_trigr
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
output dac_trigr;

///====================================
///====================================
///===== tst ==================
output [3:0]tst;
///====================================
reg [15:0]o_data;
reg [15:0]tst_dac_odata;

wire [3:0]tst;

wire o_dac_clk_p;
wire o_dac_clk_m;
wire o_dac_dclk_p;
wire o_dac_dclk_m;
wire [6:0]o_dac_dat_p;
wire [6:0]o_dac_dat_m;
///=====================================
reg [2:0]z_sync;
reg [13:0]dac_cnt;

wire sregs_we_dat;
assign sregs_we_dat=i_cs&(i_addr[13:12]==`ADDR_DAC_REGS)&(i_addr[15:14]==`ADDR_COMMON)&i_we;
reg [31:0]delay_dac;
reg [31:0]delay_dac_cnt;
reg on_cnt_delay;
wire dac_trig ;
reg  dac_trigr;
reg  ena_cnt_dac;
reg [15:0]conf;
reg [31:0]dds_val;
reg [31:0]dds_b_ph;///beg phasa
reg [15:0]len_cnt_dac;
wire [2:0]dac_rej;
wire [2:0]dac_sync_rej;
wire [1:0]ext_dac_sync_rej;

assign dac_rej=conf[2:0];
assign dac_sync_rej=conf[5:3];
assign ext_dac_sync_rej=conf[7:6];
wire en_dds;

/*
wire ena_tst_dac_out;
assign ena_tst_dac_out=conf[`B_ON_TST];
wire on_dds;
assign on_dds=conf[`B_ON_DDS];
wire en_dds;
assign en_dds=conf[`B_EN_DDS];
wire dds_ph_dat;
assign dds_ph_dat=conf[`B_DDS_PH_DAT];
*/

always @(negedge sregs_we_dat )
begin
if(i_addr[6:1] ==`OFFS_CONF)
   conf= i_data[15:0];
end

always @(negedge sregs_we_dat )
begin
if(i_addr[6:1] ==`OFFS_DAC_OUT)
   tst_dac_odata= i_data[15:0];
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
///====================================
always @(negedge sregs_we_dat )
begin
if(i_addr[6:1] ==`DELAY_DAC_L)
   delay_dac[15:0]<= i_data[15:0];
end
always @(negedge sregs_we_dat )
begin
if(i_addr[6:1] ==`DELAY_DAC_H)
   delay_dac[31:16]<= i_data[15:0];
end
///====================================
wire end_dac_cnt;
assign end_dac_cnt=(dac_cnt>=len_cnt_dac[13:0]);
wire end_delay_dac_cnt;
reg  [ 2: 0] z_end_delay_dac_cnt;
reg psk_cnt_dac;
always @*
   psk_cnt_dac <= (!z_end_delay_dac_cnt[2]) & z_end_delay_dac_cnt[1] ;
   
always @(posedge i_clk) 
begin
   if (i_clr) 
    begin
      ena_cnt_dac   <=  1'b0 ;
    end
   else 
     begin
     if (psk_cnt_dac)
        begin
         ena_cnt_dac <= 1'b1 ;
        end
      else if (end_dac_cnt)
        begin
        ena_cnt_dac   <=  1'b0 ;
        end  
      end
end

assign end_delay_dac_cnt=(delay_dac_cnt==32'h0);

always @(posedge i_clk )
if(i_clr)
    begin
    on_cnt_delay <= 1'b0;
	delay_dac_cnt <= delay_dac;				///
	end
else if(dac_trigr)
    begin
    on_cnt_delay <= 1'b1;
	delay_dac_cnt <= delay_dac;				///
	end
else if(on_cnt_delay) 
    begin
    if(end_delay_dac_cnt)
        on_cnt_delay <= 1'b0;
    else
	   delay_dac_cnt <= delay_dac_cnt-1'b1;				///
	end
	
always @(posedge i_clk) 
if (i_clr) 
    z_end_delay_dac_cnt   <= 3'b0 ;
else 
     z_end_delay_dac_cnt <= {z_end_delay_dac_cnt[1:0],end_delay_dac_cnt} ;

///====================================
always @(posedge i_clk )
if(i_clr)
	dac_cnt <= 14'b0;				///
else 
    case(dac_sync_rej)
    `AUTO_NO_SYNC:
        if(end_dac_cnt)
	       dac_cnt <= 14'b0;				///
        else
	       dac_cnt <=dac_cnt+1'b1; 
    `AUTO_SYNC:
        if((end_dac_cnt)|dac_trigr )
	       dac_cnt <= 14'b0;				///
        else
	       dac_cnt <=dac_cnt+1'b1; 
     default:   ///
         if(dac_trigr )
	       dac_cnt <= 14'b0;				///
        else if(ena_cnt_dac)
	       dac_cnt <=dac_cnt+1'b1; 
    endcase
///====================================
reg              trig_in      ;
wire             ext_trig_p   ;
wire             ext_trig_n   ;

reg  [1:0] ext_trig_dp    ;
reg  [1:0] ext_trig_dn    ;
reg  [7:0] ext_trig_debp  ;
reg  [7:0] ext_trig_debn  ;

always @(posedge i_clk )
if(i_clr)
    begin
	z_sync <= 3'b0;				///
    ext_trig_dp   <=  2'h0 ;
    ext_trig_dn   <=  2'h0 ;
    ext_trig_debp <= 8'h0 ;
    ext_trig_debn <= 8'h0 ;
	end
else
    begin
	z_sync <={z_sync[1:0],i_sync}; 
     /// look for input changes
      if ((ext_trig_debp == 8'h0) && (z_sync[1] && !z_sync[2]))
         ext_trig_debp <= `EXT_TRIG_DEB ; 
      else if (ext_trig_debp != 8'h0)
         ext_trig_debp <= ext_trig_debp - 1'h1 ;

      if ((ext_trig_debn == 8'h0) && (!z_sync[1] && z_sync[2]))
         ext_trig_debn <= `EXT_TRIG_DEB ; //
      else if (ext_trig_debn != 8'h0)
         ext_trig_debn <= ext_trig_debn - 1'h1 ;
      // update output values
      ext_trig_dp[1] <= ext_trig_dp[0] ;
      if (ext_trig_debp == 8'h0)
         ext_trig_dp[0] <= z_sync[1] ;
      ext_trig_dn[1] <= ext_trig_dn[0] ;
      if (ext_trig_debn == 8'h0)
         ext_trig_dn[0] <= z_sync[1] ;
	end
assign ext_trig_p = (ext_trig_dp == 2'b01) ;
assign ext_trig_n = (ext_trig_dn == 2'b10) ;

always @*
case(ext_dac_sync_rej)
    `FE:
        trig_in <= ext_trig_n  ; // external negative edge
    default:
        trig_in <= ext_trig_p  ; // external positive edge
endcase 

always @(posedge i_clk )
if(i_clr)
     dac_trigr <=  1'b0 ;
else
     dac_trigr <=  trig_in ;

///=============================================
assign tst= dac_cnt[3:0];
///wire [13:0]tt_odat;
reg [13:0]dac_odata;
wire [13:0]dds_out;
wire [31:0]curr_ph;

wire ps_we_ram;
wire [15:0]ps_ram_odata;
wire [12:0]dac_ram_addr;
wire [15:0]dac_ram_odata;

always @*
case(dac_rej)
    `REJ_TST_DAT:      ///test_dat_out
        dac_odata <=tst_dac_odata[13:0];
    default:
        dac_odata <=dac_ram_odata[13:0];
 endcase  
 ///==================================================   
/*
assign dac_odata= (ena_tst_dac_out)?tst_dac_odata[13:0]
                :(on_dds)?(dds_ph_dat?dac_ram_addr:dds_out)
                :dds_ph_dat?dac_cnt[13:0]:dac_ram_odata[13:0];
                */
///assign t_odat= tt_odat;
///====================================

////assign dds_out=dac_ram_odata[13:0];
assign dac_ram_addr=(dac_rej==`REJ_DDS)?curr_ph[31:19]:dac_cnt[12:0];
assign en_dds=(dac_rej==`REJ_DDS)?1'b1:1'b0;

assign ps_we_ram=i_cs&(i_addr[15:14]==`ADDR_DAC_RAM)&i_we;
mem_2kx16 mem_dac(
    .clka(i_ps_clk)
    ,.ena(1'b1)
    ,.wea(ps_we_ram)
    ,.addra(i_addr[13:1])
    ,.dina(i_data)
    ,.douta(ps_ram_odata)
    ,.clkb(i_clk)
    ,.enb(1'b1)
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
/*
ila_0 ila0(
 .clk(i_clk),
 .probe0(conf),
 .probe1(dac_cnt),
 .probe2(dac_ram_addr),
 .probe3(dac_ram_odata),
 .probe4(curr_ph[31:19]),
 .probe5(tt_odat)
 );
 */
///===================================


///====================================

genvar ii;
generate
for (ii=0;ii<7;ii=ii+1)
begin: dac_data
dac_data_out 
dac_data_( 
 .tx_clk(i_clk)
 ,.tx_data_p(dac_odata[ii])
 , .tx_data_m(dac_odata[ii+7])
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
				o_data <= tst_dac_odata;
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
            `DELAY_DAC_L:
 				o_data <= delay_dac[15:0];
           `DELAY_DAC_H:
				o_data <= delay_dac[31:16];
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

