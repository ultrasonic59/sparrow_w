///====================================
module spi_master1a #(parameter LEN = 8)
                (i_clk,i_clr,
				i_dat,
				i_psk_spi,
				o_sdat,i_sck,o_scs,o_busy,
				///======= test ================
				sndDat,endBits,set_dat,state
////				,busySpi
				);
///====================================
input i_clk,i_clr;
input [LEN-1:0]i_dat;
input i_psk_spi;
///====================================
output o_sdat;
output o_scs;
output o_busy;
input i_sck;
///====================================
///===== tst ==================
///output busySpi;
output set_dat;
output endBits;
output [LEN-1:0]sndDat;
output [3:0]state;
///=============================
function integer log2;
input integer addr;
begin
    addr = addr - 1;
    for (log2=0; addr>0; log2=log2+1)
        addr = addr >> 1;
end
endfunction
///==============================
///reg [4:0]qCntD;
///(* mark_debug = "true" *)
wire [LEN-1:0]i_dat;

///(* mark_debug = "true" *)
reg [log2(LEN)+1:0]qCntB;
////wire  coutDiv;
///====================================
///(* mark_debug = "true" *)
reg	o_sdat;
///(* mark_debug = "true" *)
reg	o_scs;
///====================================
wire o_busy;
assign o_busy = o_scs;
///====================================
///(* mark_debug = "true" *)
reg [3:0]state;
//==============================
localparam s0 = 4'd0,s1 = 4'd1,s2 = 4'd2,s3 = 4'd3,s4 = 4'd4,s5 = 4'd5,s6 = 4'd6,s7 = 4'd7;
localparam s8 = 4'd8,s9 = 4'd9,s10 = 4'd10,s11 = 4'd11,s12 = 4'd12,s13 = 4'd13;
////parameter s14 = 4'd14,s15 = 4'd15;
//=================================
///====================================
///(* mark_debug = "true" *)
reg	[LEN-1:0]sndDat;
///====================================
///====================================
///(* mark_debug = "true" *)
wire endBits;
///(* mark_debug = "true" *)
reg r_psk_spi;
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	r_psk_spi = 1'b0;				///
else
	r_psk_spi =i_psk_spi; 
///====================================
///====================================
///(* mark_debug = "true" *)
wire i_sck;
///(* mark_debug = "true" *)
reg  z_sck;
///(* mark_debug = "true" *)
wire r_sck;
///(* mark_debug = "true" *)
wire f_sck;
assign r_sck = (!z_sck)&i_sck;
assign f_sck = (!i_sck)&z_sck;

///(* mark_debug = "true" *)
wire set_dat;
assign set_dat = (state == s1);
///(* mark_debug = "true" *)
reg r_set_dat;
always @(posedge i_clk or posedge i_clr)
begin
if(i_clr)
	z_sck <= 1'b0;				//init
else
	z_sck <= i_sck;				
end
///====================================
assign endBits= (qCntB>=LEN);

///always @(negedge i_sck or posedge  r_set_dat )
always @(posedge i_clk  )
if(set_dat)
	qCntB = {(log2(LEN)+1){1'b0}};     ///5'h14;				//
else if((!endBits)&f_sck)
	qCntB = qCntB+1'b1;
///====================================
///(* mark_debug = "true" *)
wire set_cs;
assign set_cs=(state ==s2);
//========= stat machin ===========
//=================================
always @(posedge i_clk or posedge i_clr)
begin
if(i_clr)
	state = s0;				//init
else
	begin
	case(state)
		s0 :
			if(r_psk_spi)
				state =s1;	
			else			
				state =s0;	
		s1 :							/// 
			state =s2;
		s2 :							///wait cs 
		  if(o_scs)
			state =s3;
		  else
			state =s2;
		s3 :							//
           state =s4;    
		s4 :							//
			if(endBits)
				state =s5;	
			else			
				state =s4;	
		s5 :							// 
			state =s6;			
		s6 :							//wait ready spi
			state =s0;	
		default :							
			state =s0;
	endcase
	end	
end
//=================================
////always @(negedge i_sck or posedge i_clr)
always @(posedge i_clk )
begin
if(i_clr)
	o_scs = 1'b0;
else if(endBits&f_sck)	
	o_scs = 1'b0;
else if(set_cs&f_sck)
	o_scs = 1'b1;
end
///====================================
///always @(negedge i_sck  or posedge r_set_dat)
always @(negedge i_clk  )
begin
if(set_dat)
	begin
	sndDat <=i_dat;
	end
else 
    begin
    if(o_scs&f_sck)
	   begin
	   sndDat[LEN-1:0] <= {sndDat[LEN-2:0],1'b0};
	   end
    end	
///else
///	sndDat = sndDat;
end	
///====================================
always @* ///@(posedge clk)
o_sdat = sndDat[LEN-1];	
///====================================
/*
ila_0 ila0(
 .clk(i_clk),
  .probe0(r_psk_spi),
 .probe1(i_dat),
/// .probe2(sndDat),
 .probe2(qCntB),
 .probe3(o_sdat),
 .probe4(o_scs),
 .probe5(state),
 .probe6(endBits),
 .probe7(set_dat),
 .probe8(i_sck ),
 .probe9(f_sck),
 .probe10(r_sck)
 
 );
*/
endmodule
///====================================
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
///====================================
module spi_master #(parameter LEN = 8)
                (clk,clr,i_spd_div,
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
input [LEN-1:0]i_dat;
input ipsk_spi;
///====================================
input i_sdat;
output o_sdat,o_sck,o_scs,o_busy;
output [LEN-1:0]o_dat;
///====================================
///===== tst ==================
output busySpi;
output endBits;
output [7:0]sndDat;
///=============================
function integer log2;
input integer addr;
begin
    addr = addr - 1;
    for (log2=0; addr>0; log2=log2+1)
        addr = addr >> 1;
end
endfunction

reg [4:0]qCntD;
///reg [4:0]qCntB;
reg [log2(LEN)+1:0]qCntB;

wire  coutDiv;
///====================================
reg	o_sdat,o_sck,o_scs;
///====================================
wire o_busy;
assign o_busy = !o_scs;
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
///assign endBits= (qCntB==5'h10);
assign endBits= (qCntB>=(LEN*2));

always @(posedge clk )
if(sload_bits)
	qCntB= {(log2(LEN)+1){1'b0}};
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

reg [LEN-1:0] o_dat;
always @(posedge clk or posedge clr)
begin
if(clr)
	o_dat = {(LEN-1){1'b0}};
else if(endBits)	
	o_dat = sndDat;
end
///====================================
always @(posedge clk )
begin
if(qCntB== {log2(LEN){1'b0}})
	begin
	sndDat =i_dat;
	end
else if(coutDiv&(o_sck))
	begin
////	{sndDat} = {sndDat[6:0],i_sdat};
	sndDat[LEN-1:0] <= {sndDat[LEN-2:0],1'b0};
	end
end	
///====================================
always @*///@(posedge clk)
///o_sdat = sndDat[7];	
o_sdat = sndDat[LEN-1];	

///====================================
endmodule

