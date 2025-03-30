//upr time for owl
module sepia_time(i_clk,i_clr,i_psk,i_onCnt
					,o_cnt_time	,tst
					);
input i_clk;
input i_clr;
input i_psk;
input i_onCnt;
//-------------------	
output [17:0]o_cnt_time;

output tst;
//================================
wire [17:0]o_cnt_time;
reg [17:0]cnt_time;
assign o_cnt_time=cnt_time;
wire tst;
//-------------------
/*
//reg enaCnt;
reg zOnCnt;
wire bOnCnt;
assign bOnCnt = onCnt&(!zOnCnt);
wire enaCnt=zOnCnt;
wire tLd;
reg zEndCnt;
assign tLd= endCnt&(!zEndCnt);
wire ldCnt;
assign ldCnt= bOnCnt|i_psk|tLd;
assign tst = zEndCnt;
//-------------------
//-------------------	
always @(posedge clk )
zOnCnt = onCnt;
//-------------------
//-------------------	
always @(posedge clk )
zEndCnt = endCnt;
///wire clr_time;
///assign clr_time=clr|i_psk;
*/

reg r_psk;
reg z_psk;
wire clr_ipsk;
assign clr_ipsk=z_psk;
assign tst = r_psk;

always @(posedge i_psk or posedge clr_ipsk)
if(clr_ipsk)
	r_psk=1'b0;
else
	r_psk=1'b1;
always @(posedge i_clk or posedge i_clr)
if(i_clr)
	z_psk = 1'b0;				//
else 
	z_psk = r_psk;				//
//================================
always @(posedge i_clk or posedge i_clr)
begin
if(i_clr)
	cnt_time = 18'b0;				//
else if(z_psk)
	cnt_time = 18'b0;					//
else if((i_onCnt)&(!(&cnt_time)))
	cnt_time = cnt_time+1'b1;				//
end
//================================

endmodule
