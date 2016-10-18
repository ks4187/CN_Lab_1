#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.


class RF
{
    public:
        bitset<32> ReadData1, ReadData2; 
     	RF()
    	{ 
          Registers.resize(32);  
          Registers[0] = bitset<32> (0);  
        }
	
        void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
        {   
            // implement the funciton by you.                
         }
		 
	void OutputRF()
             {
               ofstream rfout;
                  rfout.open("RFresult.txt",std::ios_base::app);
                  if (rfout.is_open())
                  {
                    rfout<<"A state of RF:"<<endl;
                  for (int j = 0; j<32; j++)
                      {        
                        rfout << Registers[j]<<endl;
                      }
                     
                  }
                  else cout<<"Unable to open file";
                  rfout.close();
               
               }     
	private:
            vector<bitset<32> >Registers;
	
};

class ALU
{
      public:
             bitset<32> ALUresult;
             bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
             {   
                 // implement the ALU operations by you. 
                 return ALUresult;
               }            
};

class INSMem
{
      public:
          bitset<32> Instruction;
          INSMem()
          {       IMem.resize(MemSize); 
                  ifstream imem;
                  string line;
                  int i=0;
                  imem.open("imem.txt");
                  if (imem.is_open())
                  {
                  while (getline(imem,line))
                     {      
                        IMem[i] = bitset<8>(line);
                        i++;
                     }
                     count = i;
                  }
                  
                  else cout<<"Unable to open file";
                  imem.close();
                     
                  }
                  
                  
          bitset<32> ReadMemory (bitset<32> ReadAddress) {
              
              Instruction = (bitset<32>) (IMem[ReadAddress.to_ulong()].to_string() + IMem[ReadAddress.to_ulong() + 1].to_string() + IMem[ReadAddress.to_ulong() + 2].to_string() + IMem[ReadAddress.to_ulong() + 3].to_string());
              
              return Instruction;
              
          }
      
      private:
           vector<bitset<8> > IMem;
           int count = 0;
      
};
      
class DataMem    
{
      public:
          bitset<32> readdata;  
          DataMem()
          {
             DMem.resize(MemSize); 
             ifstream dmem;
                  string line;
                  int i=0;
                  dmem.open("dmem.txt");
                  if (dmem.is_open())
                  {
                  while (getline(dmem,line))
                       {      
                        DMem[i] = bitset<8>(line);
                        i++;
                       }
                  }
                  else cout<<"Unable to open file";
                  dmem.close();
          
          }  
          bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem) 
          {    
               
               // implement by you.
               return readdata;     
          }   
                     
          void OutputDataMem()
          {
               ofstream dmemout;
                  dmemout.open("dmemresult.txt");
                  if (dmemout.is_open())
                  {
                  for (int j = 0; j< 1000; j++)
                       {     
                        dmemout << DMem[j]<<endl;
                       }
                     
                  }
                  else cout<<"Unable to open file";
                  dmemout.close();
               
               }             
      
      private:
           vector<bitset<8> > DMem;
      
};  

/*
TODO:
 - Limit memory read to 65536
 - Decode the instructions
 - Implement different ALU operations
 - Figure out how to deal with Registers and Data Memory
 - Write logic for changing current address to jump address when jump/beq (branch on equal) instruction called
 - Change if break statement to incorporate halt instruction
*/
   
int main()
{
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;
    
    INSMem();   //INITIALIZE INSTRUCTION MEMORY
    DataMem();  //INITIALIZE DATA MEMORY
    RF();       //INITIALIZE REGISTERS
    
    bitset<32> readAddress(0); //INITIALIZE THE FIRST ADDRESS TO READ TO ALL 0's
    bitset<32> currentInstruction; //INITIALIZE THE CURRENT INSTRUCTION POINTER
    
    bitset<6> opCode;

    while (1)
	{
	    
	    currentInstruction = myInsMem.ReadMemory(readAddress);  //GET CURRENT INSTRUCTION
	    
	    if(currentInstruction.to_string() == "11111111111111111111111111111111")    //IF INSTRUCTION ALL 1's TERMINATE THE LOOP/PROGRAM
	        break;
	    else {                                                                      // ELSE PERFORM TASKS BASED ON THE INSTRUCTION
	        cout << currentInstruction.to_string() << endl; //FOR DEBUGGING
	        
	        //DECODING THE INSTRUCTION
	        opCode = (bitset<6>)( currentInstruction.to_string().substr(0, 6) );    //GET THE FIRST 6 BITS FOR THE OPCODE FROM THE CURRENT INSTRUCTION
	                             
	        cout << hex << opCode.to_ulong() << endl; //FOR DEBUGGING (Convert to hex with hex << dec/ulong)
	        
	        if((hex << opCode.to_ulong()) == 0x0){
	            //R Type Instruction
	            
	        } else
	            //I or J Type
	            
	        }
	        
	    }
	        
	        
	    readAddress = readAddress.to_ulong() + 4; //INCREMENT THE PROGRAM COUNTER SO THAT IT READS THE NEXT 32 BIT ADDRESS
	        
	    
        // Fetch - Done
        
		// If current insturciton is "11111111111111111111111111111111", then break; - Done
        
		// decode(Read RF)
		
		// Execute
		
		// Read/Write Mem
		
		// Write back to RF
		
        //myRF.OutputRF(); // dump RF (Uncomment the first part)
    }
    //myDataMem.OutputDataMem(); // dump data mem
    
    return 0;
        
}
