module u_spi8(clk,clr,i_spd_div,
				i_dat,
				ipsk_spi,i_sdat
				,o_sdat,o_sck,o_scs,o_busy,
				o_dat
				///======= test ================
				,sndDat,endBits
				,busySpi);
///====================================
input clk,clr;
input [3:0]i_spd_div;
input [7:0]i_dat;
input ipsk_spi;
///====================================
input i_sdat;
output o_sdat,o_sck,o_scs,o_busy;
output [7:0]o_dat;
///====================================
///===== tst ==================
output busySpi;
output endBits;
output [7:0]sndDat;

reg [4:0]qCntD;
reg [4:0]qCntB;
wire  coutDiv;
///====================================
reg	o_sdat,o_sck,o_scs;
///====================================
wire o_busy;
assign o_busy = !o_scs;
///====================================
///====================================
reg	[7:0]sndDat;
///====================================
wire busySpi;
assign busySpi = enCntD;
///====================================
wire endBits,enCntBits;
reg enCntD;
assign enCntBits = coutDiv;
///====================================
reg [1:0]tPsk_spi;

reg t_psk_spi;
always @(posedge ipsk_spi or posedge tPsk_spi[1])
if(tPsk_spi[1])
	t_psk_spi=1'b0;
else
	t_psk_spi=1'b1;
always @(posedge clk or posedge clr)
if(clr)
	tPsk_spi = 2'b0;				//init
else
	begin
	tPsk_spi[1] = tPsk_spi[0];				
	tPsk_spi[0] = t_psk_spi;				
	end

///====================================
wire [4:0]datDiv;
assign datDiv = {i_spd_div,1'b1};
wire bgCs;
assign bgCs =(qCntD == 5'h1);
///====================================
wire sload_div;
assign sload_div=t_psk_spi|coutDiv ;
assign coutDiv= (!(|qCntD));
always @(posedge clk )
if(sload_div)
	qCntD= datDiv;
else if(enCntD)
	qCntD= qCntD-1'b1;

///====================================
wire sload_bits;
assign sload_bits=t_psk_spi ;
assign endBits= (qCntB==5'h10);
always @(posedge clk )
if(sload_bits)
	qCntB= 5'h0;
else if(enCntBits)
	qCntB= qCntB+1'b1;
//=================================

always @(posedge clk or posedge clr)
begin
if(clr)
	o_scs = 1'b1;
else if(endBits)	
	o_scs = 1'b1;
else if(enCntD&bgCs)
	o_scs = 1'b0;
end
///====================================
always @(posedge clk or posedge clr)
begin
if(clr)
	enCntD = 1'b0;
else if(t_psk_spi)
	enCntD = 1'b1;
else if(endBits)	
	enCntD = 1'b0;
end
///====================================
always @(posedge clk or posedge clr)
begin
if(clr)
	o_sck = 1'b0;
else if(!(enCntD))
	o_sck = 1'b0;
else if(coutDiv)
	o_sck = ~o_sck;
end

reg [7:0] o_dat;
always @(posedge clk or posedge clr)
begin
if(clr)
	o_dat = 8'b0;
else if(endBits)	
	o_dat = sndDat;
end
///====================================
always @(posedge clk )
begin
if(qCntB==5'h0)
	begin
	sndDat[7:0] =i_dat;
	end
else if(coutDiv&(o_sck))
	begin
	{sndDat} = {sndDat[6:0],i_sdat};
	end
end	
///====================================
always @*///@(posedge clk)
o_sdat = sndDat[7];	
///====================================
endmodule
