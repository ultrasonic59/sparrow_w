
module red_pitaya_ugen (
   //
   input                 clk_i           ,  // clock
   input                 rstn_i          ,  // reset - active low
   // emul pipe(pwm)
   output reg [ 16-1: 0] pwm_cnf_o         ,  // values used for
   output reg [ 16-1: 0] pwm_pre_o         ,  // values used for
   output reg [ 16-1: 0] pwm_rng_o         ,  // 
   output reg [ 16-1: 0] pwm_data_o         ,  // 
   // system bus
   input      [ 32-1: 0] sys_addr        ,  // bus address
   input      [ 32-1: 0] sys_wdata       ,  // bus write data
   input                 sys_wen         ,  // bus write enable
   input                 sys_ren         ,  // bus read enable
   output reg [ 32-1: 0] sys_rdata       ,  // bus read data
   output reg            sys_err         ,  // bus error indicator
   output reg            sys_ack            // bus acknowledge signal
);

//---------------------------------------------------------------------------------
//
//  System bus connection

always @(posedge clk_i)
if (rstn_i == 1'b0) begin
   pwm_cnf_o     <= 16'h000 ;
   pwm_pre_o     <= 16'h1000 ;
   pwm_rng_o     <= 16'h1000 ;
   pwm_data_o     <= 16'h800 ;
end else begin
   if (sys_wen) begin
      if (sys_addr[19:0]==16'h100)   pwm_cnf_o <= sys_wdata[16-1: 0] ;
      if (sys_addr[19:0]==16'h104)   pwm_pre_o <= sys_wdata[16-1: 0] ;
      if (sys_addr[19:0]==16'h108)   pwm_rng_o <= sys_wdata[16-1: 0] ;
      if (sys_addr[19:0]==16'h10c)   pwm_data_o <= sys_wdata[16-1: 0] ;
   end
end

wire sys_en;
assign sys_en = sys_wen | sys_ren;

always @(posedge clk_i)
if (rstn_i == 1'b0) begin
   sys_err <= 1'b0 ;
   sys_ack <= 1'b0 ;
end else begin
   sys_err <= 1'b0 ;
   casez (sys_addr[19:0])
     20'h00100 : begin sys_ack <= sys_en;         sys_rdata <= {{32-16{1'b0}}, pwm_cnf_o}          ; end
     20'h00104 : begin sys_ack <= sys_en;         sys_rdata <= {{32-16{1'b0}}, pwm_pre_o}          ; end
     20'h00108 : begin sys_ack <= sys_en;         sys_rdata <= {{32-16{1'b0}}, pwm_rng_o}          ; end
     20'h0010c : begin sys_ack <= sys_en;         sys_rdata <= {{32-16{1'b0}}, pwm_data_o}          ; end
       default : begin sys_ack <= sys_en;         sys_rdata <=   32'h0                           ; end
   endcase
end

endmodule
