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
        bitset<32> ReadData1, ReadData2, signExtendImm; 
     	RF()
    	{ 
          Registers.resize(32);  
          Registers[0] = bitset<32> (0);  
        }
	
        void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
        {   
            if(WrtEnable == 0){
                //Read
                ReadData1 = Registers[RdReg1.to_ulong()];
                ReadData2 = Registers[RdReg2.to_ulong()];
                
                //ReadData1 = 0xFFFFFFFF;
                //ReadData2 = 0x7FFFFFFE;
                
                cout << "Read Data 1: " << ReadData1 << endl << "Read Data 2: " << ReadData2 << endl;
            } else {
                //Write
                Registers[WrtReg.to_ulong()] = WrtData;
                cout << "Register Write Data = " << Registers[WrtReg.to_ulong()] << endl << endl;
            }             
         }
         
        void signExtend(bitset<16> immData){
            bitset<16> extend(0);
            if(immData[0] == 1)
                extend.set();
            
            signExtendImm = (bitset<32>)(extend.to_string() + immData.to_string());
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
                 
                 switch(ALUOP.to_ulong()){
                     case 0x1:
                        ALUresult = (bitset<32>) (oprand1.to_ulong() + oprand2.to_ulong());
                        break;
                    case 0x3:
                        ALUresult = (bitset<32>) (oprand1.to_ulong() - oprand2.to_ulong());
                        break;
                    case 0x4:
                        ALUresult = oprand1 & oprand2;
                        break;
                    case 0x5:
                        ALUresult = oprand1 | oprand2;
                        break;
                    case 0x7:
                        ALUresult = ~ (oprand1 | oprand2);
                        break;
                    default:
                        break;
                     
                 }
                 
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
               if(readmem == 1)
                    readdata = (bitset<32>) (DMem[Address.to_ulong()].to_string() + DMem[Address.to_ulong() + 1].to_string() + DMem[Address.to_ulong() + 2].to_string() + DMem[Address.to_ulong() + 3].to_string());
               else if(writemem == 1){
                   
                   DMem[Address.to_ulong()] = (bitset<8>)(WriteData.to_string().substr(0, 8));
                   DMem[Address.to_ulong() + 1] = (bitset<8>)(WriteData.to_string().substr(8, 8));
                   DMem[Address.to_ulong() + 2] = (bitset<8>)(WriteData.to_string().substr(16, 8));
                   DMem[Address.to_ulong() + 3] = (bitset<8>)(WriteData.to_string().substr(24, 8));
                   
               }
               
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
 - Implement different ALU operations - Done
 - Figure out how to deal with Registers and Data Memory
 - Write logic for changing current address to jump address when jump/beq (branch on equal) instruction called
 - Send 001 to ALU for I type coz we are adding ReadData1 and Sign Extended Immediate
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
    bitset<32> ALUResult;
    bitset<32> DataMemResult;
    int beqFlag = 0, jFlag = 0;

    while (1)
	{
	    
	    currentInstruction = myInsMem.ReadMemory(readAddress);  //GET CURRENT INSTRUCTION
	    
	    //cout << currentInstruction.to_string() << endl; //FOR DEBUGGING
	        
	    //DECODING THE INSTRUCTION
	    opCode = (bitset<6>)( currentInstruction.to_string().substr(0, 6) );    //GET THE FIRST 6 BITS FOR THE OPCODE FROM THE CURRENT INSTRUCTION
	                             
	    //cout << hex << opCode.to_ulong() << "  "; //FOR DEBUGGING (Convert to hex with hex << dec/ulong)
	        
	    if(opCode.to_ulong() == 0x0){
	        //R Type Instruction
	        //cout << "R Type" << endl;
	        
	        //Read From Registers
	        myRF.ReadWrite((bitset<5>) currentInstruction.to_string().substr(6, 5),
	                      (bitset<5>) currentInstruction.to_string().substr(11, 5),
	                      (bitset<5>) currentInstruction.to_string().substr(16, 5),
	                      (bitset<32>) (0),
	                      (bitset<1>) (0)
	                    );
	        
	        //Send to ALU
	        ALUResult = myALU.ALUOperation((bitset<3>) currentInstruction.to_string().substr(29, 3),
	                                       myRF.ReadData1,
	                                       myRF.ReadData2
	                                       );
	        
	        //Write to Register
	        myRF.ReadWrite((bitset<5>) currentInstruction.to_string().substr(6, 5),
	                      (bitset<5>) currentInstruction.to_string().substr(11, 5),
	                      (bitset<5>) currentInstruction.to_string().substr(16, 5),
	                      ALUResult,
	                      (bitset<1>) (1)
	                    );
	        
	    } else if(opCode.to_ulong() != 0x02 && opCode.to_ulong() != 0x3F){
	        //I Type Instruction
	        //cout << "I Type" << endl;
	        
	        //Read From Registers
	        myRF.ReadWrite((bitset<5>) currentInstruction.to_string().substr(6, 5),
	                      (bitset<5>) currentInstruction.to_string().substr(11, 5),
	                      (bitset<5>) currentInstruction.to_string().substr(11, 5),
	                      (bitset<32>) (0),
	                      (bitset<1>) (0)
	                    );
	                    
	        
	        
	        //Sign Extend Imm            
	        myRF.signExtend((bitset<16>) currentInstruction.to_string().substr(16, 16));
	        
	        //Send to ALU
	        ALUResult = myALU.ALUOperation((bitset<3>) 0x1,
	                                       myRF.ReadData1,
	                                       myRF.signExtendImm
	                                       );
	        
	        
	        
	        if(opCode.to_ulong() == 0x09){  //addiu
	        
	            //Write to Register
	            myRF.ReadWrite((bitset<5>) currentInstruction.to_string().substr(6, 5),
	                      (bitset<5>) currentInstruction.to_string().substr(11, 5),
	                      (bitset<5>) currentInstruction.to_string().substr(11, 5),
	                      ALUResult,
	                      (bitset<1>) (1)
	                    );
	                    
	        }else if(opCode.to_ulong() == 0x23){
	            //Load Instruction
	            
	            //Read from Data Memory
	            DataMemResult = myDataMem.MemoryAccess(ALUResult,
	                                                   (bitset<32>)(0),
	                                                   (bitset<1>)(1),
	                                                   (bitset<1>)(0));
	                                                   
	            cout << "Data Memory: " << DataMemResult << endl;
	            
	            //Write to Register
	            myRF.ReadWrite((bitset<5>) currentInstruction.to_string().substr(6, 5),
	                      (bitset<5>) currentInstruction.to_string().substr(11, 5),
	                      (bitset<5>) currentInstruction.to_string().substr(11, 5),
	                      DataMemResult,
	                      (bitset<1>) (1)
	                    );
	            
	        } else if(opCode.to_ulong() == 0x2B){
	            //Store Instruction
	            
	            //Write to Data Memory
	            DataMemResult = myDataMem.MemoryAccess(ALUResult,
	                                                   myRF.ReadData2,
	                                                   (bitset<1>)(0),
	                                                   (bitset<1>)(1));
	        } else if(opCode.to_ulong() == 0x04){
	            //Branch on Equal (beq) Instruction
	            
	            if(myRF.ReadData1 == myRF.ReadData2){
	                
	                beqFlag = 1;
	                readAddress = readAddress.to_ulong() + 4 +
	                              ((bitset<30>)(myRF.signExtendImm.to_string().substr(0, 30))).to_ulong() +
	                              ((bitset<2>)(0)).to_ulong();
	            }
	        }
	        
	        
	    } else{
	        
	        //J Type Instruction
	        //cout << "J Type" << endl;
	        
	        
	        if(opCode.to_ulong() == 0x02){
	            //Jump Instruction
	            
	            jFlag = 1;
	            readAddress = ((bitset<4>)((bitset<32>)(readAddress.to_ulong() + 4)).to_string().substr(0, 4)).to_ulong() +
	                            ((bitset<26>)(currentInstruction.to_string().substr(6, 26))).to_ulong() +
	                            ((bitset<2>)(0)).to_ulong();
	            
	        }
	        else if(opCode.to_ulong() == 0x3F)
	            break;  //Halt Instruction
	        else{
	            cout << "UNRECOGNIZED OPCODE\nTERMINATING THE PROGRAM" << endl;
	            break;
	        }
	            
	    }
	    
	    if(!beqFlag && !jFlag)
	        readAddress = readAddress.to_ulong() + 4; //INCREMENT THE PROGRAM COUNTER SO THAT IT READS THE NEXT 32 BIT ADDRESS
	    else{
	        beqFlag = 0;
	        jFlag = 0;
	    }
		
        //myRF.OutputRF(); // dump RF (Uncomment the first part)
    }
    //myDataMem.OutputDataMem(); // dump data mem
    
    return 0;
        
}
