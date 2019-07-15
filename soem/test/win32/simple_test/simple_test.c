/** \file
 * \brief Example code for Simple Open EtherCAT master
 *
 * Usage : simple_test [ifname1]
 * ifname is NIC interface, f.e. eth0
 *
 * This is a minimal test.
 *
 * (c)Arthur Ketels 2010 - 2011
 */

#include <stdio.h>
#include <string.h>
//#include <Mmsystem.h>

#include "osal.h"
#include "ethercat.h"

#define EC_TIMEOUTMON 500

char IOmap[4096];
OSAL_THREAD_HANDLE thread1;
int expectedWKC;
boolean needlf;
volatile int wkc;
volatile int rtcnt;
boolean inOP;
uint8 currentgroup = 0;

typedef struct PACKED
{
	uint32_t value_0x02;
	uint32_t value_0x03;
	uint32_t value_0x05;
	uint32_t value_0x06;
	uint32_t value_0x07;
	uint32_t value_0xAA;
} MAC_400_OUTPUTS;

typedef struct PACKED
{
	uint32_t value_0x02;
	uint32_t value_0x0A;
	uint32_t value_0x0C;
	uint32_t value_0xAA;
	uint32_t value_0x23;
	uint32_t value_0xA9;
	uint32_t value_0x14;
	uint32_t value_0x1D;
} MAC_400_INPUTS;

/* most basic RT thread for process data, just does IO transfer */
void CALLBACK RTthread(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1,  DWORD_PTR dw2)
{
    IOmap[0]++;

	MAC_400_OUTPUTS* mac_outputs = (MAC_400_OUTPUTS*)ec_slave[1].outputs;
	mac_outputs->value_0x02 = 2;
	mac_outputs->value_0x03 = 1000000;
	mac_outputs->value_0x05 = 2700;
	mac_outputs->value_0x06 = 360;
	mac_outputs->value_0x07 = 341;

    ec_send_processdata();
    wkc = ec_receive_processdata(EC_TIMEOUTRET);
    rtcnt++;
    /* do RT control stuff here */
}

int EL7031setup(uint16 slave)
{
    int retval;
    uint16 u16val;

    // map velocity
    uint16 map_1c12[4] = {0x0003, 0x1601, 0x1602, 0x1604};
    uint16 map_1c13[3] = {0x0002, 0x1a01, 0x1a03};

    retval = 0;

    // Set PDO mapping using Complete Access
    // Strange, writing CA works, reading CA doesn't
    // This is a protocol error of the slave.
    retval += ec_SDOwrite(slave, 0x1c12, 0x00, TRUE, sizeof(map_1c12), &map_1c12, EC_TIMEOUTSAFE);
    retval += ec_SDOwrite(slave, 0x1c13, 0x00, TRUE, sizeof(map_1c13), &map_1c13, EC_TIMEOUTSAFE);

    // bug in EL7031 old firmware, CompleteAccess for reading is not supported even if the slave says it is.
    ec_slave[slave].CoEdetails &= ~ECT_COEDET_SDOCA;

    // set some motor parameters, just as example
    u16val = 1200; // max motor current in mA
//    retval += ec_SDOwrite(slave, 0x8010, 0x01, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTSAFE);
    u16val = 150; // motor coil resistance in 0.01ohm
//    retval += ec_SDOwrite(slave, 0x8010, 0x04, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTSAFE);

    // set other nescessary parameters as needed
    // .....

    while(EcatError) printf("%s", ec_elist2string());

    printf("EL7031 slave %d set, retval = %d\n", slave, retval);
    return 1;
}

int AEPsetup(uint16 slave)
{
    int retval;
    uint8 u8val;
    uint16 u16val;

    retval = 0;

    u8val = 0;
    retval += ec_SDOwrite(slave, 0x1c12, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);
    u16val = 0x1603;
    retval += ec_SDOwrite(slave, 0x1c12, 0x01, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTRXM);
    u8val = 1;
    retval += ec_SDOwrite(slave, 0x1c12, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);

    u8val = 0;
    retval += ec_SDOwrite(slave, 0x1c13, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);
    u16val = 0x1a03;
    retval += ec_SDOwrite(slave, 0x1c13, 0x01, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTRXM);
    u8val = 1;
    retval += ec_SDOwrite(slave, 0x1c13, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);

    u8val = 8;
    retval += ec_SDOwrite(slave, 0x6060, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);

    // set some motor parameters, just as example
    u16val = 1200; // max motor current in mA
//    retval += ec_SDOwrite(slave, 0x8010, 0x01, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTSAFE);
    u16val = 150; // motor coil resistance in 0.01ohm
//    retval += ec_SDOwrite(slave, 0x8010, 0x04, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTSAFE);

    // set other nescessary parameters as needed
    // .....

    while(EcatError) printf("%s", ec_elist2string());

    printf("AEP slave %d set, retval = %d\n", slave, retval);
    return 1;
}

void simpletest(char *ifname)
{
    int i, j, oloop, iloop, wkc_count, chk, slc;
    UINT mmResult;

    needlf = FALSE;
    inOP = FALSE;

   printf("Starting simple test\n");

   /* initialise SOEM, bind socket to ifname */
   if (ec_init(ifname))
   {
      printf("ec_init on %s succeeded.\n",ifname);
      /* find and auto-config slaves */


       if ( ec_config_init(FALSE) > 0 )
       {
         printf("%d slaves found and configured.\n",ec_slavecount);

         if((ec_slavecount > 1))
         {
             for(slc = 1; slc <= ec_slavecount; slc++)
             {
                 // beckhoff EL7031, using ec_slave[].name is not very reliable
                 if((ec_slave[slc].eep_man == 0x00000002) && (ec_slave[slc].eep_id == 0x1b773052))
                 {
                     printf("Found %s at position %d\n", ec_slave[slc].name, slc);
                     // link slave specific setup to preop->safeop hook
                     ec_slave[slc].PO2SOconfig = &EL7031setup;
                 }
                 // Copley Controls EAP, using ec_slave[].name is not very reliable
                 if((ec_slave[slc].eep_man == 0x000000ab) && (ec_slave[slc].eep_id == 0x00000380))
                 {
                     printf("Found %s at position %d\n", ec_slave[slc].name, slc);
                     // link slave specific setup to preop->safeop hook
                     ec_slave[slc].PO2SOconfig = &AEPsetup;
                 }
             }
         }


         ec_config_map(&IOmap);

         ec_configdc();

         printf("Slaves mapped, state to SAFE_OP.\n");
         /* wait for all slaves to reach SAFE_OP state */
         ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 4);

         oloop = ec_slave[0].Obytes;
         if ((oloop == 0) && (ec_slave[0].Obits > 0)) oloop = 1;
         if (oloop > 8) oloop = 8;
         iloop = ec_slave[0].Ibytes;
         if ((iloop == 0) && (ec_slave[0].Ibits > 0)) iloop = 1;
         if (iloop > 8) iloop = 8;

         printf("segments : %d : %d %d %d %d\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]);

         printf("Request operational state for all slaves\n");
         expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
         printf("Calculated workcounter %d\n", expectedWKC);
         ec_slave[0].state = EC_STATE_OPERATIONAL;
         /* send one valid process data to make outputs in slaves happy*/
         ec_send_processdata();
         ec_receive_processdata(EC_TIMEOUTRET);

         /* start RT thread as periodic MM timer */
         mmResult = timeSetEvent(1, 0, RTthread, 0, TIME_PERIODIC);

         /* request OP state for all slaves */
         ec_writestate(0);
         chk = 200;
         /* wait for all slaves to reach OP state */
         do
         {
            ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
         }
         while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));
         if (ec_slave[0].state == EC_STATE_OPERATIONAL )
         {
            printf("Operational state reached for all slaves.\n");
            wkc_count = 0;
            inOP = TRUE;


            /* cyclic loop, reads data from RT thread */
            for(i = 1; i <= 1000; i++)
            {
                    if(wkc >= expectedWKC)
                    {
                        printf("Processdata cycle %4d, WKC %d , O:", rtcnt, wkc);

                        for(j = 0 ; j < oloop; j++)
                        {
                            printf(" %2.2x", *(ec_slave[0].outputs + j));
                        }

                        printf(" I:");
                        for(j = 0 ; j < iloop; j++)
                        {
                            printf(" %2.2x", *(ec_slave[0].inputs + j));
                        }
                        printf(" T:%lld\r",ec_DCtime);
                        needlf = TRUE;
                    }
                    osal_usleep(5000);

            }
            inOP = FALSE;
         }
         else
         {
                printf("Not all slaves reached operational state.\n");
                ec_readstate();
                for(i = 1; i<=ec_slavecount ; i++)
                {
                    if(ec_slave[i].state != EC_STATE_OPERATIONAL)
                    {
                        printf("Slave %d State=0x%2.2x StatusCode=0x%4.4x : %s\n",
                            i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
                    }
                }
         }

         /* stop RT thread */
         timeKillEvent(mmResult);

         printf("\nRequest init state for all slaves\n");
         
		 /* request INIT state for all slaves */
		 chk = 200;
		 ec_slave[0].state = EC_STATE_INIT;
         ec_writestate(0);

		 do
		 {
			ec_statecheck(0, EC_STATE_INIT, 50000);
		 } while (chk-- && (ec_slave[0].state != EC_STATE_INIT));


        }
        else
        {
            printf("No slaves found!\n");
        }
        printf("End simple test, close socket\n");
        /* stop SOEM, close socket */
        ec_close();
    }
    else
    {
        printf("No socket connection on %s\nExcecute as root\n",ifname);
    }
}

//DWORD WINAPI ecatcheck( LPVOID lpParam )
OSAL_THREAD_FUNC ecatcheck(void *lpParam)
{
    int slave;

    while(1)
    {
        if( inOP && ((wkc < expectedWKC) || ec_group[currentgroup].docheckstate))
        {
            if (needlf)
            {
               needlf = FALSE;
               printf("\n");
            }
            /* one ore more slaves are not responding */
            ec_group[currentgroup].docheckstate = FALSE;
            ec_readstate();
            for (slave = 1; slave <= ec_slavecount; slave++)
            {
               if ((ec_slave[slave].group == currentgroup) && (ec_slave[slave].state != EC_STATE_OPERATIONAL))
               {
                  ec_group[currentgroup].docheckstate = TRUE;
                  if (ec_slave[slave].state == (EC_STATE_SAFE_OP + EC_STATE_ERROR))
                  {
                     printf("ERROR : slave %d is in SAFE_OP + ERROR, attempting ack.\n", slave);
                     ec_slave[slave].state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
                     ec_writestate(slave);
                  }
                  else if(ec_slave[slave].state == EC_STATE_SAFE_OP)
                  {
                     printf("WARNING : slave %d is in SAFE_OP, change to OPERATIONAL.\n", slave);
                     ec_slave[slave].state = EC_STATE_OPERATIONAL;
                     ec_writestate(slave);
                  }
                  else if(ec_slave[slave].state > EC_STATE_NONE)
                  {
                     if (ec_reconfig_slave(slave, EC_TIMEOUTMON))
                     {
                        ec_slave[slave].islost = FALSE;
                        printf("MESSAGE : slave %d reconfigured\n",slave);
                     }
                  }
                  else if(!ec_slave[slave].islost)
                  {
                     /* re-check state */
                     ec_statecheck(slave, EC_STATE_OPERATIONAL, EC_TIMEOUTRET);
                     if (ec_slave[slave].state == EC_STATE_NONE)
                     {
                        ec_slave[slave].islost = TRUE;
                        printf("ERROR : slave %d lost\n",slave);
                     }
                  }
               }
               if (ec_slave[slave].islost)
               {
                  if(ec_slave[slave].state == EC_STATE_NONE)
                  {
                     if (ec_recover_slave(slave, EC_TIMEOUTMON))
                     {
                        ec_slave[slave].islost = FALSE;
                        printf("MESSAGE : slave %d recovered\n",slave);
                     }
                  }
                  else
                  {
                     ec_slave[slave].islost = FALSE;
                     printf("MESSAGE : slave %d found\n",slave);
                  }
               }
            }
            if(!ec_group[currentgroup].docheckstate)
               printf("OK : all slaves resumed OPERATIONAL.\n");
        }
        osal_usleep(10000);
    }
}

char ifbuf[1024];

int main(int argc, char *argv[])
{
	printf("SOEM (Simple Open EtherCAT Master)\nSimple test\n");

	ec_adaptert* it_adapter = NULL;
	ec_adaptert* se_adapter = NULL;

	// Scan for adapters
	it_adapter = ec_find_adapters();
	se_adapter = it_adapter;
	int id = -1;

	// Print to screen
	while (it_adapter != NULL)
	{
		// Print adapter info
		id++;
		printf("%d: Description : %s, Device to use for wpcap: %s\n", id, it_adapter->desc, it_adapter->name);
		it_adapter = it_adapter->next;
	}

	// Select the one to use
	int s = -1;
	while (s < 0 || s > id)
	{
		printf("\nSelect ethernet adapter: ");
		scanf("%d", &s);
		getchar();
	}

	// Iterate to the selected adapter
	for (int i = 0; i < s; i++)
		se_adapter = se_adapter->next;

	// Print adapter info
	printf("\nSelected Adapter : %s\n\n", se_adapter->desc);

	/* create thread to handle slave error handling in OP */
	//      pthread_create( &thread1, NULL, (void *) &ecatcheck, (void*) &ctime);
	osal_thread_create(&thread1, 128000, &ecatcheck, (void*)&ctime);

	/* start cyclic part */
	simpletest(se_adapter->name);

	printf("Press any key to quit program");
	getchar();
	return (0);
}
