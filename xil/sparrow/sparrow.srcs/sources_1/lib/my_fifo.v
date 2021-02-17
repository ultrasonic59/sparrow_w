module my_fifo #(
  parameter RSZ = 13,  // RAM size 2^RSZ,
  parameter DSZ = 9  // Data size ,

)(
   //
   i_clk    ,  
   i_clr    ,  //
   i_data   ,  // 
   i_we		,
   i_rd		,
   o_data	,
   o_cnt	,
   o_fifo_full	,
   o_fifo_empty,
   i_clr_fifo
   );
   input  i_clk;
   input  i_clr;
   input  [ DSZ-1: 0] i_data;
   input  i_we;
   input  i_rd;
   output [ DSZ-1: 0] o_data;
   output [RSZ-1:0]o_cnt;
   output o_fifo_full;
   output o_fifo_empty;
   input  i_clr_fifo;

wire o_fifo_full;
wire o_fifo_empty;
wire [RSZ-1:0]o_cnt;
reg [ DSZ-1: 0] o_data;

///========================================================
reg   [DSZ-1: 0] ram_buf [0:(1<<RSZ)-1] ;
reg   [ RSZ-1: 0] wr_addr  ;
reg   [ RSZ-1: 0] rd_addr  ;
reg   [ RSZ-1: 0] cnt  ;
reg z_we;
reg z_rd;
wire e_we;
wire e_rd;
assign e_we=(!i_we)&z_we;
assign e_rd=(!i_rd)&z_rd;
wire fifo_full;
wire fifo_empty;
assign fifo_empty=!(|cnt);
assign fifo_full=&cnt;
assign o_fifo_full= fifo_full;
assign o_fifo_empty=fifo_empty;
assign o_cnt=cnt;

///========================================================
always @(posedge i_clk)
begin
if(i_clr) 
	begin
	z_we<=1'b0;
	z_rd<=1'b0;
	end
else
	begin
	z_we<=i_we;
	z_rd<=i_rd;
	end
end	

always @(posedge i_clk)
begin
if(i_clr) 
	begin
	cnt<={RSZ{1'b0}};
	end
else if(i_clr_fifo)
	begin
	cnt<={RSZ{1'b0}};
	end
else if(e_we&(!fifo_full))
	begin
	cnt<=cnt+1'b1;
	end
else if(e_rd&(!fifo_empty))
	begin
	cnt<=cnt-1'b1;
	end
end	

always @(posedge i_clk)
begin
if(i_clr) 
	begin
	wr_addr<={RSZ{1'b0}};
	end
else if(i_clr_fifo)
	begin
	wr_addr<={RSZ{1'b0}};
	end
else if(e_we&(!fifo_full))
	begin
	wr_addr<=wr_addr+1'b1;
	end
end	
always @(posedge i_clk)
begin
if(i_clr) 
	begin
	rd_addr<={RSZ{1'b0}};
	end
else if(i_clr_fifo)
	begin
	rd_addr<={RSZ{1'b0}};
	end
else if(e_rd&(!fifo_empty))
	begin
	rd_addr<=rd_addr+1'b1;
	end
end	
always @(posedge i_clk)
begin
if(i_rd)
	begin
	o_data<=ram_buf[rd_addr];
	end
end	
always @(posedge i_clk)
begin
if(i_we)
	begin
	ram_buf[wr_addr]<=i_data;
	end
end	
 
///========================================================

endmodule