`include "sparrow_mmap.v"

module u_sniffer(i_ps_clk
            ,i_clk,i_clr
            ,i_cs,i_we,i_rd,i_addr
			,i_data,o_data
///======= usb ===========================
            ,i_dm,i_dp	
 		    ,tst);
///====================================
input i_ps_clk;
input i_clk,i_clr;
input i_cs;
input i_we;
input i_rd;
input [15:0]i_addr;
input [15:0]i_data;
output [15:0]o_data;
///======= usb ===========================
input i_dm;
input i_dp;			

///input [9:0]i_dat;
///====================================
///====================================
///===== tst ==================
output [3:0]tst;
///====================================
reg [15:0]o_data;
///=================================
reg [7:0]rdata;
reg rdata_ready;
wire EOP;
wire T_EOP;
wire usb_reset;

////==========================================================
reg [15:0]conf;
wire [15:0]fifo_cnt;
reg [15:0]cur_fifo_odata;
wire sregs_we_dat;
assign sregs_we_dat=i_cs&(i_addr[13:12]==`ADDR_SNIFF_REGS)&(i_addr[15:14]==`ADDR_COMMON)&i_we;
wire sregs_rd_dat;
assign sregs_rd_dat=i_cs&(i_addr[13:12]==`ADDR_SNIFF_REGS)&(i_addr[15:14]==`ADDR_COMMON)&i_rd;
reg rd_fifo;

reg clr_fifo;
wire [8:0]t_dat_fifo;
reg [8:0]i_dat_fifo;
wire we_fifo96;
reg r_we_fifo96;
reg r_we_fifo_ps;
wire we_fifo;
wire [8:0]o_dat_fifo;
wire o_fifo_full;
wire o_fifo_empty;
wire [12:0]o_fifo_cnt;
assign fifo_cnt={3'b0,o_fifo_cnt};
assign t_dat_fifo=(usb_reset)?9'h1dd:(EOP)?9'h1ee:{1'b0,rdata};
wire t_wr_fifo;
assign t_wr_fifo=rdata_ready|usb_reset|EOP;
assign we_fifo96=(o_fifo_full)?1'b0:t_wr_fifo;
assign we_fifo=r_we_fifo_ps;
///===============================
always @(negedge sregs_we_dat )
begin
if(i_addr[6:1] ==`OFFS_CONF)
   conf<= i_data[15:0];
end
always @(posedge i_ps_clk )
begin
if((i_addr[6:1] ==`OFFS_FIFO_CLR)&(sregs_we_dat))
   clr_fifo<= i_data[0];
else  
   clr_fifo<= 1'b0;
end
always @(posedge i_ps_clk )
begin
if((i_addr[6:1] ==`OFFS_FIFO_RD)&(sregs_rd_dat))
    begin
    cur_fifo_odata={7'b0,o_dat_fifo};
   rd_fifo<= 1'b1;
   end
else  
   rd_fifo<= 1'b0;
end

always @(posedge i_clk  )
begin
if(we_fifo96)
    begin
    r_we_fifo96<=1'b1;
    i_dat_fifo<=t_dat_fifo;
    end
else if(r_we_fifo_ps)
    r_we_fifo96<=1'b0;
end
always @(posedge i_ps_clk )
begin
   r_we_fifo_ps<=r_we_fifo96;
end
///===========================================================
reg [3:0]rbyte_cnt;
//fix current USB line values
reg [7:0]dp_input;
reg [7:0]dm_input;
reg speed = 1'b0;
reg [2:0]receiver_cnt=0;
reg last_fixed_line=0;
reg receiver_enabled=0;
wire enable;
reg [15:0]rst_counter;
wire in_reset; 
wire [1:0]line_input;
assign T_EOP= (i_dp==1'b0)&(i_dm==1'b0);
assign enable=1;

always @(posedge i_clk)
begin
	dp_input <= { dp_input[6:0], i_dp };
	dm_input <= { dm_input[6:0], i_dm };
end


//enable receiver on raising edge of signal line
always @(posedge i_clk or posedge EOP )
begin
if(EOP)
	receiver_enabled <= 1'b0;
else
    if(in_reset)
		receiver_enabled <= 1'b0;
	else
	if( line_input[0] )
		receiver_enabled <= 1'b1 & enable;
end

//both dp/dm signals are in zero during reset/se0/no-attach
wire end_of_se0; 
assign end_of_se0 = ( (dp_input | dm_input) == 8'h01) ;
///assign end_of_se0 = (dm_input  == 8'h01)&&(dp_input  == 8'h01) ;
assign EOP  = end_of_se0;

always @(posedge i_clk)
if( end_of_se0 )
  speed <= dp_input[0];
assign in_reset = rst_counter[15];
always @(posedge i_clk)
if(dp_input | dm_input)
    rst_counter <= 0;
else
    if(~in_reset)
        rst_counter <= rst_counter+1;

reg [1:0]rst_sr;
always @(posedge i_clk)
	rst_sr <= {rst_sr[0],in_reset};

//impulse
assign usb_reset = (rst_sr==2'b01);
//we do not have possibility to work with differential inputs
//we just select line to spy on depending on speed
assign line_input = speed ? dm_input[1:0] : dp_input[1:0];

//change of dp/dm lines defines strobing
wire lcross; 
assign lcross = (line_input[0] ^ line_input[1]);
//generate clocks with this counter
reg [5:0]clk_counter;
reg  strobe;

//make strobe impulse
always @(posedge i_clk)
begin		
//every edge on line resynchronizes receiver clock
if(lcross | ~receiver_enabled)
    begin
	clk_counter <= 0;
	end
else
	if(speed)
		clk_counter[2:0] <= (clk_counter[2:0] + 1'b1);
	else
		clk_counter <= (clk_counter + 1'b1);
end
//catch counter somewhere in the middle of bit transfer
always @(posedge i_clk)
strobe <= speed ? clk_counter[2:0]==3'h2 : clk_counter==6'h20;
	
//next bit received is ONE if no signal change or ZERO if signal changed
wire next_bit; 
assign next_bit = (last_fixed_line == line_input[1]);
//calc number of sequental ONEs in received bit stream
reg [2:0]num_ones;
always @(posedge i_clk)
begin
if(strobe & receiver_enabled)
	begin
	if(next_bit)
		num_ones <= num_ones + 1'b1;
	else
		num_ones <= 0;
	end
end
//flag which mean that zero should be removed from bit stream
wire do_remove_zero; 
assign do_remove_zero = (num_ones==6);
//receiver process
always @(posedge i_clk or posedge EOP )
begin
if(EOP)
    begin
	//kind of reset
	receiver_cnt <= 0;
	rbyte_cnt <= 0;
	last_fixed_line <= 1'b0;
	rdata <= 0;
	rdata_ready <= 1'b0;
	end
else
	begin
	if(strobe & receiver_enabled & (!do_remove_zero))
		begin
			//decode NRZI
			//shift-in ONE  if older and new values are same
			//shift-in ZERO if older and new values are different
			//BUT (bitstuffling) do not shift-in one ZERO after 6 ONEs
			rdata  <= { next_bit, rdata[7:1] };
			receiver_cnt <= receiver_cnt + 1'b1;	
		end

		//set write-enable signal (write into receiver buffer)
		rdata_ready <= (receiver_cnt == 7) & (strobe & receiver_enabled & (!do_remove_zero));
		
		//remember last fixed signal,
		if(strobe & receiver_enabled)
			last_fixed_line <= line_input[1];
		//count nomber of received bytes
		if(rdata_ready)
			rbyte_cnt <= rbyte_cnt + 1'b1;
	end
end
////===============================================================================
/*
fifo_sniff fifo_ (
    .clk(i_ps_clk)
    ,.srst(clr_fifo)
    ,.din(i_dat_fifo)
    ,.wr_en(we_fifo)
    ,.rd_en(rd_fifo)
    ,.dout(o_dat_fifo)
    ,.full(o_fifo_full)
    ,.overflow()
    ,.empty(o_fifo_empty)
    ,.underflow()
    ,.data_count(o_fifo_cnt)
  );
*/  
 my_fifo #(.RSZ(13),
        .DSZ(9)) 
    fifo_ (
    .i_clk(i_ps_clk)
    ,.i_clr(i_clr)
    ,.i_clr_fifo(clr_fifo)
    ,.i_data(i_dat_fifo)
    ,.i_we(we_fifo)
    ,.i_rd(rd_fifo)
    ,.o_data(o_dat_fifo)
    ,.o_fifo_full(o_fifo_full)
    ,.o_fifo_empty(o_fifo_empty)
    ,.o_cnt(o_fifo_cnt)
  );
///================================ 
  ///===============================
ila_0 ila0(
 .clk(i_ps_clk),
  .probe0(clr_fifo),
 .probe1(i_dat_fifo),
 .probe2(we_fifo),
 .probe3(rd_fifo),
 .probe4(o_dat_fifo),
 .probe5(o_fifo_full),
 .probe6(o_fifo_empty),
  .probe7(o_fifo_cnt),
  .probe8(dp_input),
  .probe9(dm_input),
  .probe10(i_dp),
  .probe11(i_dm),
  .probe12(EOP),
  .probe13(usb_reset),
  .probe14(t_wr_fifo)
 );
 
//====================================
always @*
begin
case(i_addr[15:14])         ///1
	`ADDR_COMMON:
	   case(i_addr[13:12])     ///2
	   `ADDR_SNIFF_REGS:
	   		case(i_addr[6:1])     ///3
			`OFFS_SNIFF_CONF:
				o_data <= conf;
			`OFFS_FIFO_CNT:
				o_data <= fifo_cnt;
			`OFFS_FIFO_STAT:
				o_data <= {o_fifo_full,o_fifo_empty,fifo_cnt[13:0]};
			`OFFS_FIFO_RD:
				o_data <= cur_fifo_odata;
             default:    
	           o_data <= 16'h3113;
          endcase         ///3
        default:    
	       o_data <= 16'h2332;
	    endcase        ///2
    default:    
      o_data <= 16'h1111;
    endcase        ///1
end   
////===============================================================================

endmodule
