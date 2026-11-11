	Important!!
	Read the File_Structure.txt file to get an overview of the contents
	
	Basic example of FreeRTOS in the MicroZedBoard
	==============================================
	==============================================

	1)	Create a hardware platform
	==============================

		On Xilinx SDK
		new-> application project
			a) Fill the form as next:
				Project Name: "FreeRTOS_generated"
				Target hardware:
					select "MicroZed_hw_platform(pre-defined)"
				Boards support package:
					Select Create new
			b) Leave the rest as default
			c) Click on next
			
		select "empty application"
		Click on finish
		
	Will generate a dummy application but it will also create the needed BSP and the hardware description.
		
	2)	Import the FreeRTOS in your workspace
	=========================================

		Right click on project Explorer
			import
				General
					Existing project into Workspace
						Browse
							Select the path from the given sources or downloaded from the git repo
							
		Change referenced BSP
			Right click on the new imported "FreeRTOS" project
				Click on "Change referenced BSP"
					Select: FreeRTOS_generated_BSP
			Right click on the new imported "FreeRTOS" project
				run as...
					GDB

	3) See console logs (Optional)
	==============================
	
		Right click on the new imported "FreeRTOS" project
			Properties
				run/debug settings
					FreeRTOS.elf
						Edit
							STDIO connection
								Check Connect STDIO to console
									Port: ... check to which port is your USB connected
									Baud rate: 115200
		
		
