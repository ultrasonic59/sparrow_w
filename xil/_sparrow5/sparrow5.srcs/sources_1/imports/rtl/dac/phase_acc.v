`timescale 1 ns / 1 ps


module phase_acc
    (
    input         clk_i,
    input         clr_i,
    input         phase_en_i,
    input         sync_i,
    input  [31:0]b_phase_i,
    input  [31:0]dds_val_i,
    output [31:0] phase_o
    );
   localparam MAX_CNT_BL=1000;
    
     reg [31:0] phase_reg;
     wire ena_sync;
     reg [9:0]cnt_bl;
     assign ena_sync= !(|cnt_bl);
 ///=========================================== 
      always @(posedge clk_i)
     if(clr_i)
        cnt_bl <=10'b0;
    else if(sync_i)    
       cnt_bl <= MAX_CNT_BL;
    else if(!ena_sync)    
        cnt_bl <= cnt_bl - 1'b1;
  
     always @(posedge clk_i)
     if(clr_i)
        phase_reg <=32'b0;
    else if(sync_i&ena_sync)    
       phase_reg <= b_phase_i;
    else if(phase_en_i)    
        phase_reg <= phase_reg + dds_val_i;
 ///===========================================   
    assign phase_o = phase_reg;
 ///===========================================   
    
endmodule
