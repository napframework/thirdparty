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
#include <inttypes.h>

#include "ethercat.h"

#define EC_TIMEOUTMON 500

char IOmap[4096];
OSAL_THREAD_HANDLE thread1;
int expectedWKC;
boolean needlf;
volatile int wkc;
boolean inOP;
uint8 currentgroup = 0;

typedef struct PACKED
{
	uint16_t value_6040;
	int32_t value_607A;
} servo_outputs;

int servo_setup(uint16 slave)
{
	// Create bitmask
	uint16_t control_word = 0;
	control_word |= 1UL << 0;
	control_word |= 1UL << 1;
	control_word |= 1UL << 2;
	control_word |= 1UL << 3;
	control_word |= 1UL << 4;
	control_word |= 1UL << 5;
	control_word |= 1UL << 6;
	control_word |= 1UL << 7;
	control_word |= 1UL << 8;
	control_word |= 1UL << 9;
	control_word |= 1UL << 10;
	control_word |= 1UL << 11;
	control_word |= 1UL << 12;
	control_word |= 1UL << 13;
	control_word |= 1UL << 14;
	control_word |= 1UL << 15;

	int32_t motor_pos = 0;
	uint32_t motor_mode = 2;
	uint32_t max_velocity = 700;
	uint32_t max_acceleration = 360;
	uint32_t max_torque = 341;

	int retval = 0;

	//retval += ec_SDOwrite(slave, 0x6040, 0x00, FALSE, sizeof(control_word), &control_word, EC_TIMEOUTSAFE);

	// Set some motor parameters
 	retval += ec_SDOwrite(slave, 0x2012, 0x02, FALSE, sizeof(motor_mode), &motor_mode, EC_TIMEOUTSAFE);
	retval += ec_SDOwrite(slave, 0x2012, 0x05, FALSE, sizeof(max_velocity), &max_velocity, EC_TIMEOUTSAFE);
	retval += ec_SDOwrite(slave, 0x2012, 0x06, FALSE, sizeof(max_acceleration), &max_acceleration, EC_TIMEOUTSAFE);
	retval += ec_SDOwrite(slave, 0x2012, 0x07, FALSE, sizeof(max_torque),  &max_torque,  EC_TIMEOUTSAFE);
	retval += ec_SDOwrite(slave, 0x2012, 0x03, FALSE, sizeof(motor_pos), &motor_pos, EC_TIMEOUTSAFE);

	printf("AEP slave %d set, retval = %d\n", slave, retval);

	return retval;
}


void simpletest(char *ifname)
{
    int i, j, oloop, iloop, chk;
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
		 ec_slave[1].PO2SOconfig = &servo_setup;

		 // Setup IO map
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
         /* request OP state for all slaves */
         ec_writestate(0);
         chk = 200;
         /* wait for all slaves to reach OP state */
         do
         {
            ec_send_processdata();
            ec_receive_processdata(EC_TIMEOUTRET);
            ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
         }
         while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));
         
		 if (ec_slave[0].state == EC_STATE_OPERATIONAL )
         {
            printf("Operational state reached for all slaves.\n");
            inOP = TRUE;
            
			/* cyclic loop */
            for(i = 1; i <= 10000; i++)
            {
               ec_send_processdata();
               wkc = ec_receive_processdata(EC_TIMEOUTRET);

                    if(wkc >= expectedWKC)
                    {
                        printf("Processdata cycle %4d, WKC %d , O:", i, wkc);

						// Print current master outputs
                        for(j = 0 ; j < oloop; j++)
                        {
                            printf(" %2.2x", *(ec_slave[0].outputs + j));
                        }

						// Print current master inputs
                        printf(" I:");
                        for(j = 0 ; j < iloop; j++)
                        {
                            printf(" %2.2x", *(ec_slave[0].inputs + j));

							servo_outputs* outputs = (servo_outputs*)ec_slave[0].inputs;
							
                        }
                        printf(" T:%"PRId64"\r",ec_DCtime);
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
            printf("\nRequest init state for all slaves\n");
            ec_slave[0].state = EC_STATE_INIT;
            /* request INIT state for all slaves */
            ec_writestate(0);
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

OSAL_THREAD_FUNC ecatcheck( void *ptr )
{
    int slave;
    (void)ptr;                  /* Not used */

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
