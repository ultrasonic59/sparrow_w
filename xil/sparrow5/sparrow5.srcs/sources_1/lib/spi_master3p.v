///====================================
///====== spi 3pin ext cs ========
module spi_master3p #(parameter LEN = 8)
                (i_clk,i_clr,i_spd_div
				,i_dir,i_dat
				,ipsk_spi,io_sdat
				,o_sck,o_busy
				,o_dat,i_sdat
				///======= test ================
				,o_sdat
				,sndDat,endBits
				,busySpi);
///====================================
input i_clk,i_clr;
input [4:0]i_spd_div;
input i_dir;    ///1 wr,0-rd
input [LEN-1:0]i_dat;
input ipsk_spi;
///====================================
inout io_sdat;
output o_sck,o_busy;
output [LEN-1:0]o_dat;
input i_sdat;
///====================================
///===== tst ==================
output o_sdat;
output busySpi;
output endBits;
output [LEN-1:0]sndDat;
///=============================
function integer log2;
input integer addr;
begin
    addr = addr - 1;
    for (log2=0; addr>0; log2=log2+1)
        addr = addr >> 1;
end
endfunction

reg [5:0]qCntD;
///reg [4:0]qCntB;
reg [log2(LEN)+1:0]qCntB;

wire  coutDiv;
///====================================
tri io_sdat;
wire isdat;
reg	o_sdat,o_sck;
assign io_sdat=(i_dir)?o_sdat:1'bz;
assign isdat=(i_dir)?i_sdat:io_sdat;
///====================================
reg o_busy;
///====================================
///====================================
reg	[LEN-1:0]sndDat;
///====================================
reg enCntD;
wire busySpi;
assign busySpi = enCntD;
///====================================
wire endBits,enCntBits;
assign enCntBits = coutDiv;
///====================================

reg [1:0]tPsk_spi;

reg t_psk_spi;
always @(posedge ipsk_spi or posedge tPsk_spi[1])
if(tPsk_spi[1])
	t_psk_spi=1'b0;
else
	t_psk_spi=1'b1;
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	tPsk_spi = 2'b0;				//init
else
	begin
	tPsk_spi[1] = tPsk_spi[0];				
	tPsk_spi[0] = t_psk_spi;				
	end

///====================================
wire [5:0]datDiv;
assign datDiv = {i_spd_div,1'b1};
wire bgCs;
assign bgCs =(qCntD == 6'h1);
///====================================
wire sload_div;
assign sload_div=t_psk_spi|coutDiv ;
assign coutDiv= (!(|qCntD));
always @(posedge i_clk )
if(sload_div)
	qCntD= datDiv;
else if(enCntD)
	qCntD= qCntD-1'b1;

///====================================
wire sload_bits;
assign sload_bits=t_psk_spi ;
///assign endBits= (qCntB==5'h10);
assign endBits= (qCntB>=(LEN*2));

always @(posedge i_clk or posedge i_clr)
if(i_clr)
	qCntB= {(log2(LEN)+1){1'b0}};
else if(sload_bits)
	qCntB= {(log2(LEN)+1){1'b0}};
else if(enCntBits)
	qCntB= qCntB+1'b1;
//=================================

always @(posedge i_clk or posedge i_clr)
begin
if(i_clr)
	o_busy = 1'b0;
else if(t_psk_spi)
	o_busy = 1'b1;
else if(endBits)	
	o_busy = 1'b0;
///else if(enCntD&bgCs)
///	o_busy = 1'b1;
end
///====================================
always @(posedge i_clk or posedge i_clr)
begin
if(i_clr)
	enCntD = 1'b0;
else if(t_psk_spi)
	enCntD = 1'b1;
else if(endBits)	
	enCntD = 1'b0;
end
///====================================
always @(posedge i_clk or posedge i_clr)
begin
if(i_clr)
	o_sck = 1'b0;
else if(!(enCntD))
	o_sck = 1'b0;
else if(coutDiv)
	o_sck = ~o_sck;
end

reg [LEN-1:0] o_dat;
always @(posedge i_clk or posedge i_clr)
begin
if(i_clr)
	o_dat = {(LEN-1){1'b0}};
else if(endBits)	
	o_dat = sndDat;
end
///====================================
///wire
always @(posedge i_clk )
begin
if(qCntB== {log2(LEN){1'b0}})
	begin
	sndDat =i_dat;
	end
else if(coutDiv&(o_sck))
	begin
////	{sndDat} = {sndDat[6:0],i_sdat};
///	sndDat[LEN-1:0] <= {sndDat[LEN-2:0],1'b0};
	sndDat[LEN-1:0] <= {sndDat[LEN-2:0],isdat};
	end
end	
///====================================
always @*///@(posedge clk)
///o_sdat = sndDat[7];	
o_sdat = sndDat[LEN-1];	

///====================================
endmodule
