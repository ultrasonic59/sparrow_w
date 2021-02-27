`include "sparrow_mmap.v"
///====================================
module u_spi(i_clk,i_clr
			///==== ps ========
			,i_cs,i_we,i_addr
			,i_data,o_data
			///=== spi ======
			,o_cs_spi,i_sdat
			,io_sdat,o_sck,o_busy
			///======= test ================
			,tst
				);
///====================================
input i_clk,i_clr;
input i_we;
input [15:0]i_data;
input [6:1]i_addr;
input i_cs;
output [15:0]o_data;
///====================================
output [1:0]o_cs_spi;
input i_sdat;
inout io_sdat;
output o_sck;
output o_busy;
///===== tst ==================
output [3:0]tst;
///=============================
///====================================
reg [15:0]o_data;
///====================================
reg [1:0]o_cs_spi;
tri io_sdat;
wire t_sck;
wire o_sck;
wire o_busy;
reg [7:0]spd_spi;
///===== tst ==================
wire [3:0]tst;
///===============================
reg [7:0]spi_cnf;
wire inv_sck;
assign inv_sck=spi_cnf[0];
assign o_sck=t_sck^inv_sck;

wire set_sregs;
assign set_sregs=i_cs&i_we;

always @(posedge i_clk or posedge i_clr)
if(i_clr)
  spd_spi<= 8'b0;
else if((i_addr[6:1]==`OFFS_SPI_SPD)&set_sregs)
  spd_spi<= i_data[7:0];

always @(posedge i_clk or posedge i_clr)
if(i_clr)
  o_cs_spi[0]<= 1'b0;
else if((i_addr[6:1]==`OFFS_SPI_CS_A)&set_sregs)
  o_cs_spi[0]<= i_data[0];
always @(posedge i_clk or posedge i_clr)
if(i_clr)
  o_cs_spi[1]<= 1'b0;
else if((i_addr[6:1]==`OFFS_SPI_CS_B)&set_sregs)
  o_cs_spi[1]<= i_data[0];
  
   
 always @(posedge i_clk or posedge i_clr)
if(i_clr)
  spi_cnf<= 8'b0;
else if((i_addr[6:1]==`OFFS_SPI_CNF)&set_sregs)
  spi_cnf<= i_data[7:0];

///==============================
reg spi_dir;
reg [7:0]spi_idata;

always @(posedge i_clk)
if((i_addr[6:1] ==`OFFS_SPI_WR_DATA ) &set_sregs)
  begin
	spi_idata<=i_data[7:0];
	spi_dir<=1'b1;
  end
 else if((i_addr[6:1] ==`OFFS_SPI_REQ_DATA ) &set_sregs)
 	spi_dir<=1'b0;
 	
reg t_cs_spi;
reg psk_spi;

always @(posedge i_clk)
if((i_addr[6:1] ==`OFFS_SPI_WR_DATA )| (i_addr[6:1] ==`OFFS_SPI_REQ_DATA ))
  begin
	t_cs_spi<=set_sregs;
  end
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	psk_spi <= 1'b0;
else 
	psk_spi <= t_cs_spi;
wire [7:0]spi_odat;
///wire tst_i_sdat;
wire tst_o_sdat;

///==============================
spi_master3p #(.LEN(8))
        spi_
         (.i_clk(i_clk),.i_clr(i_clr),.i_spd_div(spd_spi[4:0])
		,.i_dir(spi_dir),.i_dat(spi_idata)
		,.ipsk_spi(psk_spi),.io_sdat(io_sdat)
		,.o_sck(t_sck),.o_busy(o_busy)
		,.o_dat(spi_odat),.i_sdat(i_sdat)
		///======= test ================
		,.o_sdat(tst_o_sdat)
		,.sndDat(),.endBits()
		,.busySpi());
///==============================
always @*
case(i_addr[6:1])
    `OFFS_SPI_SPD:
	   o_data<={8'b0,spd_spi};
    `OFFS_SPI_CS_A:
	   o_data<={14'b0,o_busy,o_cs_spi[0]};
    `OFFS_SPI_CS_B:
	   o_data<={14'b0,o_busy,o_cs_spi[1]};
    `OFFS_SPI_RD_DATA:
	   o_data<={8'b0,spi_odat};
    `OFFS_SPI_CNF:
	   o_data<={8'b0,spi_cnf};
	   
endcase
///===============================
assign tst[0]=psk_spi;
assign tst[1]=t_cs_spi;
assign tst[2]=set_sregs;
assign tst[3]=spi_dir;
/*
///===============================
ila_0 ila0(
 .clk(i_clk),
  .probe0(psk_spi),
 .probe1(o_cs_spi),
 .probe2(o_busy),
 .probe3(o_sck),
 .probe4(tst_i_sdat),
 .probe5(spi_odat),
 .probe6(spi_idata),
  .probe7(tst_o_sdat)

 );
///===================================
*/
endmodule
///====================================
