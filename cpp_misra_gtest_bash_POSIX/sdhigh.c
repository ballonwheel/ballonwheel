#include <stdlib.h>








/******************************************************************************
 * expected global hw declarations
 ******************************************************************************/
//extern err_t SD_init(uint32_t * disksize);
//extern err_t SD_rd(void *d, unsigned long offset);
//extern err_t SD_wr(const void *s, unsigned long offset);


/********* *********************************************************************
 * global declarations (needed by others)
 ******************************************************************************/
err_t sd_result = OK;
err_t filesys_result = OK;;






const char	vol_id_v1[] = "__55HN__";
const char	vol_id_win[] = "FORMATED   ";

const char * const _eeof = ">>>EEOF";
const char * const _enof = ">>>ENOF";
const uint8_t empty_fat[] = 			{/*0xF8,*/ 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00};
//const uint8_t empty_fat[] = 			{0xF8, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00};




ej_operation_err_t filesys_mount(uint_least8_t t)
{
mount_start:
	debug.puttxt("filesys_mount()");
	if(t & FSYS_MOUNT_MOUNT){
		debug.puttxt("mount");

		/*******************************************************************************
		 * SD filesys chk --> format if that has win filesys
		 ******************************************************************************/
		filesys_result = filesys_format_chk();
		debug.puthex("filesys_format_chk() return: ", filesys_result);
		if(filesys_result == UNFORMATTED){
			debug.puttxt("UNFORMATTED");
			//debug.puttxt("SD_FATWIN");
			if((filesys_result = filesys_format()) != E_EJ_OK){ //empty win fat32 found, label must be "FORMATED"
				debug.puthex("filesys_format() error: ", filesys_result);
				return filesys_result;
			}
			debug.puttxt("BC filesys formating OK!");
		}
		else if(filesys_result == E_EJ_SD_FATFAIL){//it found neither win fat32 (or label must have been "FORMATED") nor new BC fat32 // old BC filesys is not accepted
			debug.puttxt("E_EJ_SD_FATFAIL");
			goto mount_error;
		}
		else{
			debug.puttxt("E_EJ_SD_FATBC");
		}


		#if 0
		/*******************************************************************************
		 * test: prepare SD
		 ******************************************************************************/
		{
			data_sector_t	*p = &sectors[RAWTMP];	

			
			if(SD_rd(p, SD_1ST_DIRDATA_SECTOR) != E_EJ_OK)
				longjmp(jmp_env, SETJMP_SDRD_ERR);

			sys.zno = 2345;
			strcpy(GLOBAL_EJ_MSN_P, "123456789");
			sprintf(p->root_dir[0].dir_name, "%c%c%c%c%04dEJ ",  GLOBAL_EJ_MSN_P[MSNLEN - 4],  GLOBAL_EJ_MSN_P[MSNLEN - 3],  GLOBAL_EJ_MSN_P[MSNLEN - 2],  GLOBAL_EJ_MSN_P[MSNLEN - 1], GLOBAL_EJ_SYS_ZNO + 1);
			//memcpy(p->root_dir[0].dir_name, "BC_SYS0xBIN", SHORTNAMELEN);//CAPITAL LETTERS!!!
			//p->root_dir[0].dir_attr = ATTR_HIDDEN | ATTR_SYSTEM;
			p->root_dir[0].dir_attr = ATTR_READ_ONLY;
			p->root_dir[0].dir_ntres = 0x10;//info got from a win formated sd 
			p->root_dir[0].dir_crttimetenth = 0x0e;//info got from a win formated sd
			p->root_dir[0].dir_clust_h = 0;
			p->root_dir[0].dir_clust_l = SD_RESERVED_CLUSTERS + DIRCLUSTERS;
			p->root_dir[0].dir_crttime =
			p->root_dir[0].dir_wrttime = 0x784f;
			p->root_dir[0].dir_crtdate =
			p->root_dir[0].dir_wrtdate = 0x378d;
			p->root_dir[0].dir_filesize = 4;

			SD_wr(p, SD_1ST_DIRDATA_SECTOR);

			
			strcpy((char*)&p->raw, "haho");
			SD_wr(p, SD_1ST_DATA_SECTOR);






			if(SD_rd(p, SD_1ST_DIRDATA_SECTOR + 1) != E_EJ_OK)
				longjmp(jmp_env, SETJMP_SDRD_ERR);
			sys.zno = 2346;
			strcpy(sysid.msn, "123456789");
			sprintf(p->root_dir[0].dir_name, "%c%c%c%c%04dEJ ", sysid.msn[MSNLEN - 4], sysid.msn[MSNLEN - 3],	sysid.msn[MSNLEN - 2], sysid.msn[MSNLEN - 1], sys.zno + 1);
			//memcpy(p->root_dir[0].dir_name, "BC_SYS0xBIN", SHORTNAMELEN);//CAPITAL LETTERS!!!
			//p->root_dir[0].dir_attr = ATTR_HIDDEN | ATTR_SYSTEM;
			p->root_dir[0].dir_attr = ATTR_READ_ONLY;
			p->root_dir[0].dir_ntres = 0x10;//info got from a win formated sd 
			p->root_dir[0].dir_crttimetenth = 0x0e;//info got from a win formated sd
			p->root_dir[0].dir_clust_h = 0;
			p->root_dir[0].dir_clust_l = SD_RESERVED_CLUSTERS + DIRCLUSTERS + 1;
			p->root_dir[0].dir_crttime =
			p->root_dir[0].dir_wrttime = 0x784f;
			p->root_dir[0].dir_crtdate =
			p->root_dir[0].dir_wrtdate = 0x378d;
			p->root_dir[0].dir_filesize = 4;

			SD_wr(p, SD_1ST_DIRDATA_SECTOR + 1);

			
			strcpy((char*)&p->raw, "hah2");
			SD_wr(p, SD_1ST_DATA_SECTOR + 1 * SECPCLUSTER);
			


			if(SD_rd(p, SD_1ST_DIRDATA_SECTOR + 2) != E_EJ_OK)
				longjmp(jmp_env, SETJMP_SDRD_ERR);
			sys.zno = 2347;
			strcpy(sysid.msn, "123456789");
			sprintf(p->root_dir[0].dir_name, "%c%c%c%c%04dEJ ", sysid.msn[MSNLEN - 4], sysid.msn[MSNLEN - 3],	sysid.msn[MSNLEN - 2], sysid.msn[MSNLEN - 1], sys.zno + 1);
			//memcpy(p->root_dir[0].dir_name, "BC_SYS0xBIN", SHORTNAMELEN);//CAPITAL LETTERS!!!
			//p->root_dir[0].dir_attr = ATTR_HIDDEN | ATTR_SYSTEM;
			p->root_dir[0].dir_attr = ATTR_READ_ONLY;
			p->root_dir[0].dir_ntres = 0x10;//info got from a win formated sd 
			p->root_dir[0].dir_crttimetenth = 0x0e;//info got from a win formated sd
			p->root_dir[0].dir_clust_h = 0;
			p->root_dir[0].dir_clust_l = SD_RESERVED_CLUSTERS + DIRCLUSTERS + 2;
			p->root_dir[0].dir_crttime =
			p->root_dir[0].dir_wrttime = 0x784f;
			p->root_dir[0].dir_crtdate =
			p->root_dir[0].dir_wrtdate = 0x378d;
			p->root_dir[0].dir_filesize = 4 * 64;


			sprintf(&p->root_dir[1].dir_name[1], "OPENED");

			SD_wr(p, SD_1ST_DIRDATA_SECTOR + 2);

			
			memset(p, 0, 512);		
			sha1_init(&sys.context);
			
			strcpy((char*)&p->raw, "123456789012345678901234567890123456789012\r\n");
			debug.memdump("&p->raw:", &p->raw, 44);
			sha1_write(&sys.context, (unsigned char *) &p->raw, 44);
			debug.memdump("sys.context:    ", &sys.context, sizeof(sys.context));

			strcpy((char*)&p->raw[64 * 1], "123456789012345678901234567890123456789012\r\n");
			debug.memdump("&p->raw[64 * 1]:", &p->raw[64 * 1], 44);
			sha1_write(&sys.context, (unsigned char *) &p->raw[64 * 1], 44);
			debug.memdump("sys.context:    ", &sys.context, sizeof(sys.context));

			strcpy((char*)&p->raw[64 * 2], "123456789012345678901234567890123456789012\r\n");
			debug.memdump("&p->raw[64 * 2]:", &p->raw[64 * 2], 44);
			sha1_write(&sys.context, (unsigned char *) &p->raw[64 * 2], 44);
			debug.memdump("sys.context:    ", &sys.context, sizeof(sys.context));

			strcpy((char*)&p->raw[64 * 3], "123456789012345678901234567890123456789012\r\n");
			debug.memdump("&p->raw[64 * 3]:", &p->raw[64 * 3], 44);
			sha1_write(&sys.context, (unsigned char *) &p->raw[64 * 3], 44);
			debug.memdump("sys.context:    ", &sys.context, sizeof(sys.context));

			//sha1_final(&sys.context);
			//debug.memdump("sys.context:    ", &sys.context, sizeof(sys.context));
			
			debug.memdump("sd data", p, 512);
			SD_wr(p, SD_1ST_DATA_SECTOR + 2 * SECPCLUSTER);
			
		}
		#endif



		/*******************************************************************************
		 * we have valid BC filesys (empty or with data)
		 ******************************************************************************/
		{
			//chking stuff here
			data_sector_t	*p = GLOBAL_EJ_ARRAY_TMP_P;	
			uint32_t i, j, k;
			
			debug.puthex("GLOBAL_EJ_USED_OPENED: ", GLOBAL_EJ_USED_OPENED);					
			GLOBAL_EJ_USED_CLOSED = 0;
			GLOBAL_EJ_FIRST_Z = 0;
			GLOBAL_EJ_LAST_Z = 0;
			GLOBAL_EJ_CURR_Z = 0;

			debug.puttxt("**********************");
			debug.puttxt("get bc dir info & chk");


			//used space: count the size of ej files
			for(j = 0; j < DIRCLUSTERS; j++){
				for(i = 0; i < SECPCLUSTER; i++){
					debug.puthex("j: ", j);
					debug.puthex("i: ", i);
					debug.puthex("i + j * SECPCLUSTER + SD_1ST_DIRDATA_SECTOR: ", i + j * SECPCLUSTER + SD_1ST_DIRDATA_SECTOR);
				
					if(SD_rd(p, i + j * SECPCLUSTER + SD_1ST_DIRDATA_SECTOR) != E_EJ_OK)
						longjmp(jmp_env, SETJMP_SDRD_ERR);
					debug.memdump("sd data", p, 512);
					
					//first file
					if(!j && !i){
						if(p->root_dir[0].dir_name[0] != 0xe5){
							//chk "EJ " attribute
							if(memcmp(&p->root_dir[0].dir_name[8], "EJ ", 3)){
								debug.puttxt("first ej attribute fail: E_EJ_SD_DIRBCFAIL");
								filesys_result =  E_EJ_SD_DIRBCFAIL;		
								goto mount_error;
							}
						
							//chk AP
							if(memcmp(&p->root_dir[0].dir_name, &sysid.msn[MSNLEN - 4], 4)){
								debug.puttxt("first ej name AP fail: E_EJ_SD_DIRBCFAIL");
								filesys_result =  E_EJ_SD_DIRBCFAIL;		
								goto mount_error;
							}

							//chk chars: ascii numbers are needed
							for(k = 0; k < 8; k++){
								if((p->root_dir[0].dir_name[k] < 0x30) || (p->root_dir[0].dir_name[k] > 0x39)){
									debug.puttxt("first ej name num fail: E_EJ_SD_DIRBCFAIL");
									filesys_result =  E_EJ_SD_DIRBCFAIL;		
									goto mount_error;
								}
							}
							
							//set first_z
							//should be known that previously stored the name of ej:
							//sprintf(p->root_dir[0].dir_name, "%c%c%c%c%04d.EJ", sys.msn[MSNLEN - 4], sys.msn[MSNLEN - 3],	sys.msn[MSNLEN - 2], sys.msn[MSNLEN - 1], sys.zno + 1);
							p->root_dir[0].dir_name[8] = '\0';//name[8] not used any more, only name[0]
							ejadmin.first_z = atoi(&p->root_dir[0].dir_name[4]);
							debug.puthex("ejadmin.first_z: ", ejadmin.first_z);					
					
						}
						else{
							//situations of following data see BC_core_io_filesys.h -> ej status
							GLOBAL_EJ_FIRST_Z = GLOBAL_EJ_SYS_ZNO + 1;
							GLOBAL_EJ_LAST_Z = GLOBAL_EJ_SYS_ZNO + 1;
							GLOBAL_EJ_CURR_Z = -1;

							debug.puttxt("first ej not found");
							goto fileend;
							
						}
					}


					//chk other files in sector; must be deleted
					for(k = 1; k < 16; k++){
						if(p->root_dir[k].dir_name[0] != 0xe5){
							debug.puttxt("other file found: E_EJ_SD_DIRBCFAIL");
							filesys_result =  E_EJ_SD_DIRBCFAIL;		
							goto mount_error;
						}
					}

					if(p->root_dir[0].dir_name[0] != 0xe5){
						//set last_z
						//should be known that previously stored the name of ej:
						//sprintf(p->root_dir[0].dir_name, "%c%c%c%c%04d.EJ", sys.msn[MSNLEN - 4], sys.msn[MSNLEN - 3],	sys.msn[MSNLEN - 2], sys.msn[MSNLEN - 1], sys.zno + 1);
						p->root_dir[0].dir_name[8] = '\0';//name[8] not used any more, only name[0]
						GLOBAL_EJ_LAST_Z = atoi(&p->root_dir[0].dir_name[4]);
						debug.puthex("GLOBAL_EJ_LAST_Z: ", GLOBAL_EJ_LAST_Z);					
						
						//set curr_z	
						GLOBAL_EJ_CURR_Z = -1;
						if(!memcmp(&p->root_dir[1].dir_name[1], "OPENED", 6))
							GLOBAL_EJ_CURR_Z = GLOBAL_EJ_LAST_Z;
						debug.puthex("GLOBAL_EJ_CURR_Z: ", GLOBAL_EJ_CURR_Z);						


						//chk filesize
						{
						uint32_t rem;
							debug.puthex("p->root_dir[0].dir_filesize: ", p->root_dir[0].dir_filesize);					
							if((rem = p->root_dir[0].dir_filesize % MAXLINELEN)){
								debug.puthex("E_EJ_SD_DIRBCFAIL filesize: ", rem);						
								filesys_result =  E_EJ_SD_DIRBCFAIL;		
								goto mount_error;
								//?? p->root_dir[0].dir_filesize += MAXLINELEN - rem;
							}						
						}
						
						//used_closed and used_opened
						if(GLOBAL_EJ_CURR_Z == -1){//closed ej
							GLOBAL_EJ_USED_CLOSED += (p->root_dir[0].dir_filesize / CLUSTER_SIZE + ((p->root_dir[0].dir_filesize % CLUSTER_SIZE) ? 1 : 0)) * CLUSTER_SIZE;
							debug.puthex("GLOBAL_EJ_USED_CLOSED: ", GLOBAL_EJ_USED_CLOSED);					
						}
						else{//opened ej
							//mount comes from 3 places:
							//1: ram clr
							//     ej closed: GLOBAL_EJ_USED_OPENED will be 0 above
							//     ej opened: recovery() will set the GLOBAL_EJ_USED_OPENED
							//2: dayopen (closed ej or do not have any):
							//     GLOBAL_EJ_USED_OPENED will be 0 above
							//3: normal turning on the BC
							//     we have valid stored GLOBAL_EJ_USED_OPENED info from flash
							//note: last dir info is not used 
							//     GLOBAL_EJ_USED_OPENED = p->root_dir[0].dir_filesize;
							if(t & FSYS_MOUNT_RAMCLR)
								GLOBAL_EJ_USED_OPENED = 0;//recovery need
							debug.puthex("GLOBAL_EJ_USED_OPENED: ", GLOBAL_EJ_USED_OPENED);					
						}
					}
					else
						goto fileend;


				}
			}
		}
mount_error:
		GLOBAL_EJ_STATUS &=  ~STATUS_EJ_OK;
		return filesys_result;
fileend:
		if(GLOBAL_EJ_CURR_Z == EJ_NOTOPEN)
			GLOBAL_EJ_USED_OPENED = 0;

		GLOBAL_EJ_STATUS |= STATUS_EJ_OK;
		GLOBAL_EJ_STATUS &= ~STATUS_EJ_DIRTY;
		

		debug.puthex("GLOBAL_EJ_USED_CLOSED: ", GLOBAL_EJ_USED_CLOSED);					
		debug.puthex("GLOBAL_EJ_USED_OPENED: ", GLOBAL_EJ_USED_OPENED);					
		debug.puthex("GLOBAL_EJ_FIRST_Z: ", GLOBAL_EJ_FIRST_Z);					
		debug.puthex("GLOBAL_EJ_LAST_Z: ",  GLOBAL_EJ_LAST_Z);					
		debug.puthex("GLOBAL_EJ_CURR_Z: ",  GLOBAL_EJ_CURR_Z);						
	}

	//last zno chk
	//see BC_core_io_filesys.h ->"ej status"
	if((GLOBAL_EJ_LAST_Z != GLOBAL_EJ_SYS_ZNO + 1) && (GLOBAL_EJ_LAST_Z != GLOBAL_EJ_SYS_ZNO)){
		filesys_result = E_EJ_LAST_Z_MISMATCH;
		return log_error(TX_ERR_NOTHING, ERR_EJ);
	}

	//fm & sd emptiness chk
	if(!FTM && !FILESYS_BCFORMATED_EMPTY){
		filesys_result = E_EJ_UNEXPECTED_BEING_EJ;
		return log_error(TX_ERR_NOTHING, ERR_EJ);
	}
	
	//ej closing interrupt chk
	if(!(t & FSYS_MOUNT_RAMCLR))
		if(!GLOBAL_EJ_DAYOPENED && !GLOBAL_EJ_DAYOPENING && (GLOBAL_EJ_CURR_Z != EJ_NOTOPEN)){//not dayopened && ej opened
			filesys_admin(FSYS_ADMIN_CLOSE);	
			debug.puttxt("goto mount_start;");						
			goto mount_start;
		}
	

	/*******************************************************************************
	 * opened ej hash chk 
	 ******************************************************************************/
	if(t & FSYS_MOUNT_HASHCHK){
		if(GLOBAL_EJ_CURR_Z != EJ_NOTOPEN){
			if(filesys_hash_chk(GLOBAL_EJ_CONTEXT_P) != E_EJ_OK){			
				GLOBAL_EJ_STATUS &= ~STATUS_EJ_OPENEDEJHASH_OK;
				#if defined(DEBUG_ONLINE)
				debug.puttxt("Error: check_ej_hash() != E_EJ_OK");
				#endif
				//log_error(TX_ERR_NOTHING, ERR_EJ);
				return filesys_result = E_EJ_HASH_MISMATCH;
			}

		}

		GLOBAL_EJ_STATUS |=  STATUS_EJ_OPENEDEJHASH_OK;
	}
	
	
		
		
	/*******************************************************************************
	 * reset the real ej boudaries //previously that could be set to test
	 ******************************************************************************/
	ej_full_setup();
	ej_full_chk();

	debug.puttxt("filesys_mount() end");

	return filesys_result = E_EJ_OK;
}



/*******************************************************************************
 * filesys_admin(uint_least8_t t)
 * desc: 
 *      admin ej data dir sector
 *      function can be called again after interruption
 * type : FSYS_ADMIN_OPEN, FSYS_ADMIN_CLOSE, FSYS_ADMIN_UPDATE
 * FSYS_ADMIN_OPEN: mark ej to be opened
 * FSYS_ADMIN_CLOSE: mark ej to be not opened any more
 * FSYS_ADMIN_UPDATE: only refresh ej size
 ******************************************************************************/
ej_operation_err_t filesys_admin(uint_least8_t t)
{
err_t r = OK;
data_sector_t	*p = _P;
uint_least8_t k;
	
	
	memset(p, 0, SECTOR_SIZE);

	/***********************************************************************
	 *
	 **********************************************************************/
	sprintf(p->root_dir[0].dir_name, "%c%c%c%c%04dEJ ", GLOBAL_EJ_MSN_P[MSNLEN - 4], GLOBAL_EJ_MSN_P[MSNLEN - 3],	GLOBAL_EJ_MSN_P[MSNLEN - 2], GLOBAL_EJ_MSN_P[MSNLEN - 1], (t == FSYS_ADMIN_CLOSE) ? (GLOBAL_EJ_SYS_ZNO) : (GLOBAL_EJ_SYS_ZNO + 1));
	p->root_dir[0].dir_attr = ATTR_READ_ONLY;
	p->root_dir[0].dir_ntres = 0x10;//info got from a win formated sd 
	p->root_dir[0].dir_crttimetenth = 0x0e;//info got from a win formated sd
	p->root_dir[0].dir_clust_h = (SD_RESERVED_CLUSTERS + DIRCLUSTERS + GLOBAL_EJ_USED_CLOSED / CLUSTER_SIZE) / 65536;
	p->root_dir[0].dir_clust_l = (SD_RESERVED_CLUSTERS + DIRCLUSTERS + GLOBAL_EJ_USED_CLOSED / CLUSTER_SIZE) % 65536;
	p->root_dir[0].dir_crttime =
	p->root_dir[0].dir_wrttime = time_bcd_fat();
	p->root_dir[0].dir_crtdate =
	p->root_dir[0].dir_wrtdate = date_bcd_fat();
	p->root_dir[0].dir_filesize = GLOBAL_EJ_USED_OPENED;

	debug.puthex("SD_RESERVED_CLUSTERS + DIRCLUSTERS + GLOBAL_EJ_USED_CLOSED / 65536 :",  (SD_RESERVED_CLUSTERS + DIRCLUSTERS + GLOBAL_EJ_USED_CLOSED / CLUSTER_SIZE) / 65536);
	debug.puthex("SD_RESERVED_CLUSTERS + DIRCLUSTERS + GLOBAL_EJ_USED_CLOSED % 65536 :",  (SD_RESERVED_CLUSTERS + DIRCLUSTERS + GLOBAL_EJ_USED_CLOSED / CLUSTER_SIZE) % 65536);

	debug.puthex("FIRST_SECTOR_OF_OPENEDEJ + GLOBAL_EJ_USED_OPENED / SECTOR_SIZE: ", FIRST_SECTOR_OF_OPENEDEJ + GLOBAL_EJ_USED_OPENED / SECTOR_SIZE);

	for(k = 1; k < 16; k++){
		p->root_dir[k].dir_name[0] = 0xe5;
	}

	/***********************************************************************
	 *
	 **********************************************************************/
	if(((t & ~FSYS_ADMIN_PWFAIL) == FSYS_ADMIN_UPDATE) || ((t & ~FSYS_ADMIN_PWFAIL) == FSYS_ADMIN_OPEN)){
		memcpy(&p->root_dir[1].dir_name[1], "OPENED", 6);

		//situations of following data see BC_core_io_filesys.h -> ej status
		GLOBAL_EJ_CURR_Z = GLOBAL_EJ_LAST_Z = GLOBAL_EJ_SYS_ZNO + 1;
	}
	if((t & ~FSYS_ADMIN_PWFAIL) == FSYS_ADMIN_CLOSE){
		memcpy(&p->root_dir[1].dir_name[1], "CLOSED", 6);

		//situations of following data see BC_core_io_filesys.h -> ej status
		GLOBAL_EJ_CURR_Z = EJ_NOTOPEN;
		GLOBAL_EJ_LAST_Z = GLOBAL_EJ_SYS_ZNO;
		
		GLOBAL_EJ_USED_CLOSED += (p->root_dir[0].dir_filesize / CLUSTER_SIZE + ((p->root_dir[0].dir_filesize % CLUSTER_SIZE) ? 1 : 0)) * CLUSTER_SIZE;

		save((flash_t*)&sys);
	}

	/***********************************************************************
	 *
	 **********************************************************************/
	debug.puthex("SD_wr():SD_1ST_DIRDATA_SECTOR + GLOBAL_EJ_LAST_Z - GLOBAL_EJ_FIRST_Z :", SD_1ST_DIRDATA_SECTOR + GLOBAL_EJ_LAST_Z - GLOBAL_EJ_FIRST_Z);
	//debug.memdump("sd data:", p, SECTOR_SIZE);
	if(SD_wr(p, SD_1ST_DIRDATA_SECTOR + GLOBAL_EJ_LAST_Z - GLOBAL_EJ_FIRST_Z) != E_EJ_OK)
		longjmp(jmp_env, SETJMP_SDWR_ERR);

	/***********************************************************************
	 *
	 **********************************************************************/
	GLOBAL_EJ_STATUS &= ~STATUS_EJ_DIRTY;
	
	debug.puthex("filesys_admin() end:", r);
	if(!(t & FSYS_ADMIN_PWFAIL))
		pwfail_enable();
	return r; //after FSYS_ADMIN_CLOSE filessys_mount() comes
}











































/*******************************************************************************
 * internal prototypes
 ******************************************************************************/
/*******************************************************************************
 * ej_operation_err_t filesys_format_chk(void)
 * desc: only fat32 filesystem is supported
 * test cases:
 *   FAT16: recognized from MBR or BOOT sector, not supported 	: error
 *   FAT32: win formated but root name is not "FORMATED"		: error
 *   FAT32: there is no MBR but sector 0 is valid boot sector, win formated and empty   : OK
 *   FAT32: win formated, there is MBR and boot sector  is sector 1 and empty : OK
 *   FAT32: p->boot_sector.fs_version should be checked in different operating system
 *   NTFS, EXT and other filesystem should be tested whether that will be return with error or not
 *   old BC filesystem found : error
 *   label "FORMATED"  is found but FAT is not empty : error
 ******************************************************************************/
ej_operation_err_t filesys_format_chk(void)
{
data_sector_t	*p = GLOBAL_EJ_ARRAY_TMP_P;
int32_t boot_sector;
int32_t sec_per_cluster;
int32_t reserved_sectors;
int32_t media;
int32_t total_sectors;
int32_t sectors_per_fat;
int32_t dir_sector;
int32_t format;
int32_t i,j,k;

	debug.puttxt("filesys_format_chk()");


	//is sector 0 partition sector?
	debug.puttxt("SECTOR 0 / MBR chk");
	if(SD_rd(p, 0) != E_EJ_OK)
		longjmp(jmp_env, SETJMP_SDRD_ERR);
	
	debug.memdump("", p, 512);
	if(p->partition_table._filler[0] == 0x00){
		if(p->partition_table.tables[0].type != 0x0b){//check FAT32
			debug.puthex("p->partition_table.tables[0].type error:", p->partition_table.tables[0].type);
			return E_EJ_SD_FATFAIL;
		}
		boot_sector = p->partition_table.tables[0].lba;
		if(SD_rd(p, boot_sector) != E_EJ_OK)
			longjmp(jmp_env, SETJMP_SDRD_ERR);
		if((p->boot_sector.jmp_boot[0] != 0xeb) && (p->boot_sector.jmp_boot[0] != 0xe9)){//check boot sector
			debug.puthex("boot sector jmp code error:", p->boot_sector.jmp_boot[0]);
			debug.memdump("", p, 512);
			return E_EJ_SD_FATFAIL;
		}
	}
	//win formated card which previously did not have partition table sector
	else if((p->boot_sector.jmp_boot[0] == 0xeb) || (p->boot_sector.jmp_boot[0] == 0xe9)){
		boot_sector = 0;
		debug.puthex("boot_sector (from sec 0):", boot_sector);
	}
	else{
		debug.puthex("p->partition_table._filler[0]: ", p->partition_table._filler[0]);
		debug.puttxt("unrecognized filesystem!");
		return E_EJ_SD_FATFAIL;
	}

	//here p has boot sector
	debug.puttxt("SECTOR x / MBR chk");
	debug.puthex("x => boot_sector:", boot_sector);
	debug.memdump("", p, 512);


	//set system parameters
	sec_per_cluster = p->boot_sector.sectors_per_cluster;
	debug.puthex("sec_per_cluster: ", sec_per_cluster);

	reserved_sectors = p->boot_sector.reserved_sectors;
	debug.puthex("reserved_sectors: ", p->boot_sector.reserved_sectors);

	if(p->boot_sector.bytes_per_sector != 512){
		debug.puthex("p->boot_sector.bytes_per_sector error:", p->boot_sector.bytes_per_sector);
		return E_EJ_SD_FATFAIL;
	}
	if(p->boot_sector.num_fats != 2){
		debug.puthex("p->boot_sector.num_fats:", p->boot_sector.num_fats);
		return E_EJ_SD_FATFAIL;
	}
	if(p->boot_sector._root_entries){//FAT32 must be 0
		debug.puthex("p->boot_sector._root_entries error:", p->boot_sector._root_entries);
		return E_EJ_SD_FATFAIL;
	}
	if(p->boot_sector._totsec16){//FAT32 must be 0
		debug.puthex("p->boot_sector._totsec16 error:", p->boot_sector._totsec16);
		return E_EJ_SD_FATFAIL;
	}
	if(p->boot_sector._fatsz16){//FAT32 must be 0
		debug.puthex("p->boot_sector._fatsz16 error:", p->boot_sector._fatsz16);
		return E_EJ_SD_FATFAIL;
	}
	media = p->boot_sector.media;//should be 0xf8; below checked in FAT[0]
	debug.puthex("media: ", media);
	
	total_sectors = p->boot_sector.total_sectors;//for FAT32
	debug.puthex("total_sectors: ", total_sectors);

	sectors_per_fat = p->boot_sector.sectors_per_fat;
	debug.puthex("sectors_per_fat: ", sectors_per_fat);

	if(p->boot_sector.fs_version){//must be version 0 //to do different win should be checked to format SD card
		debug.puthex("p->boot_sector.fs_version error:", p->boot_sector.fs_version);
		return E_EJ_SD_FATFAIL;
	}
	if(p->boot_sector.root_cluster != 2){//should be 2
		debug.puthex("p->boot_sector.root_cluster error:", p->boot_sector.root_cluster);
		return E_EJ_SD_FATFAIL;
	}
	
	dir_sector = boot_sector + reserved_sectors + 2 * sectors_per_fat;
	debug.puthex("dir_sector: ", dir_sector);
	
	if(p->boot_sector.fsinfo_sector != 1){
		debug.puthex("p->boot_sector.fsinfo_sector error:", p->boot_sector.fsinfo_sector);
		return E_EJ_SD_FATFAIL;
	}
	if(p->boot_sector.backup_bootrec != 6){//must be 6
		debug.puthex("p->boot_sector.backup_bootrec error:", p->boot_sector.backup_bootrec);
		return E_EJ_SD_FATFAIL;
	}
	if(p->boot_sector.boot_signature != 0x29){//must be 0x29
		debug.puthex("p->boot_sector.boot_signature error:", p->boot_sector.boot_signature);
		return E_EJ_SD_FATFAIL;
	}
	//if(memcmp(p->boot_sector.volume_label, "blabla     ")) //do not care label bec. win does not care neither to update dir name
	//	return E_EJ_SD_FATFAIL;
	if(memcmp(p->boot_sector.file_system_type, "FAT32   ", 8)){
		debug.puttxt("p->boot_sector.file_system_type error:");
		//debug.puttxt(p->boot_sector.file_system_type"0");
		return E_EJ_SD_FATFAIL;
	}

	if(p->boot_sector.signature != 0xaa55){
		debug.puthex("p->boot_sector.signature error:", p->boot_sector.signature);
		return E_EJ_SD_FATFAIL;
	}
 
	//fs info chk
	if(SD_rd(p, boot_sector + 1) != E_EJ_OK)
		longjmp(jmp_env, SETJMP_SDRD_ERR);
	if(p->fsinfo_sector.lead_signature != 0x41615252){
		debug.puthex("p->fsinfo_sector.lead_signature error:", p->fsinfo_sector.lead_signature);
		return E_EJ_SD_FATFAIL;
	}
	
	if(p->fsinfo_sector.struct_signature != 0x61417272){
		debug.puthex("p->fsinfo_sector.struct_signature error:", p->fsinfo_sector.struct_signature);
		return E_EJ_SD_FATFAIL;
	}
	
	if(p->fsinfo_sector.signature != 0xaa550000){
		debug.puthex("p->fsinfo_sector.signature error:", p->fsinfo_sector.signature);
		return E_EJ_SD_FATFAIL;
	}

	if(SD_rd(p, boot_sector + 2) != E_EJ_OK)
		longjmp(jmp_env, SETJMP_SDRD_ERR);

	if(p->fsinfo_sector.signature != 0xaa550000){
		debug.puthex("p->fsinfo_sector.signature error2:", p->fsinfo_sector.signature);
		return E_EJ_SD_FATFAIL;
	}
 
	
	//check root label
	if(SD_rd(p, dir_sector) != E_EJ_OK)
		longjmp(jmp_env, SETJMP_SDRD_ERR);
	
	if(memcmp(vol_id_win, p->root_dir[0].dir_name, 11)){//name of "FORMATED" check
		if(memcmp(vol_id_v1, p->root_dir[0].dir_name, 11) || (boot_sector != 1)){//"name of "BLACKCAT101" and boot place checked
			debug.puttxt("vol_id_v1 error:");
			return E_EJ_SD_FATFAIL;
		}
		else{
			format = E_EJ_SD_FATBC;
			debug.puttxt("E_EJ_SD_FATBC");
		}
	}
	else{
		format = E_EJ_SD_FATWIN;
		debug.puttxt("E_EJ_SD_FATWIN");
	}
	
	//check formatted system
	if(format == E_EJ_SD_FATBC){	
		//sould be checked
		debug.puttxt("BC filesystem chk should be here");

		return E_EJ_OK;
	}
	else{ //E_EJ_SD_FATWIN
		//FAt 1,2 empty
		debug.puttxt("WIN filesystem emptiness chk");
		debug.puttxt("FAT chk");
		for(k = boot_sector + reserved_sectors; k < boot_sector + reserved_sectors + 2 * sectors_per_fat; k += sectors_per_fat){//FAT1,2
			debug.puthex("k:", k);
			for(i = 0; i < sectors_per_fat; i++){//FATx sectors
				debug.puthex("i:", i);
				WATCHDOG_RESET;
				if(SD_rd(p, k + i) != E_EJ_OK)
					longjmp(jmp_env, SETJMP_SDRD_ERR);
				//F8FFFF0F FFFFFFFF FFFFFF0F 00000000
				if(!i){
					debug.puthex("i0:", i);
					#if 0
					if(memcmp(empty_fat, p, 16)){
						debug.memdump("empty_fat", empty_fat, 16);
						debug.memdump("p", p, 16);
						return E_EJ_SD_FATFAIL;
					}
					#else
					debug.memdump("p->raw[1]:", &p->raw[1], 15);
					if(((uint8_t)p->raw[0] != (uint8_t)media) || memcmp(empty_fat, &p->raw[1], 15)){
						debug.memdump("empty_fat", empty_fat, 15);
						debug.memdump("p->raw[1]:", &p->raw[1], 15);
						return E_EJ_SD_FATFAIL;
					}
					#endif
				}
				for(j = 0; j < 512; j++){//in sectors
					if(!i && !j){
						j = 16;
						debug.puthex("i00:j:", j);
					}
					if(p->raw[j]){
						debug.puthex("p->raw[j]:", p->raw[j]);
						return E_EJ_SD_FATFAIL;
					}
				}
			
			}
		}

		//dir entries only root; 
		//FAT checked before: cluster 2 is dir 
		debug.puttxt("dir entries");
		debug.puthex("sec_per_cluster:", sec_per_cluster);
		debug.puthex("dir_sector:", dir_sector);
		for(i = 0; i < sec_per_cluster; i++){//sector
			WATCHDOG_RESET;
			debug.puthex("i:", i);
			if(SD_rd(p, dir_sector + i) != E_EJ_OK)
				longjmp(jmp_env, SETJMP_SDRD_ERR);
			for(k = 0; k < 16 * 32; k++){//dir
				debug.puthex("k:", k);
				if(!i && !k){//root dir ignored
					k = 32;
					debug.puthex("i000:k:", k);
				}
				if(p->raw[k]){
					debug.puthex("p->raw[k]:", p->raw[k]);
					return E_EJ_SD_FATFAIL;		
				}
			}
		}	

		debug.puttxt("return E_EJ_BCUNFORMATTED");
		return E_EJ_BCUNFORMATTED;	
	}
}

/***************************************************************************
 * err_t filesys_format(void)
 * desc:
 *      WARNING!
 *      this function writes MBR, boot sector, FAT and directory entries... 
 *      !! stored data will be lost !! 
 *      SD_init() is needed before calling
 *      filesys_format_chk() is needed before calling
 *
 ***************************************************************************/
#if 0
#define SYSTEMFILES 15
#endif

err_t filesys_format(void)
{
int	i, j, k;
unsigned int nextcluster;
data_sector_t *p = GLOBAL_EJ_ARRAY_TMP_P;
	
	debug.puttxt("filesys_format()");
	
	
	/***************************************************************************
	* MBR / partiton table
	***************************************************************************/
	debug.puttxt("**** MBR ****");

	memset(p->raw, 0, SECTOR_SIZE);
	p->partition_table.tables[0].type = 0x0b;
	p->partition_table.tables[0].lba = 1;

	p->partition_table.tables[0].length = NSECTORS;
	p->partition_table.signature = 0xaa55;
	if(SD_wr(p, SD_PARTAB_SECTOR) != OK)
		longjmp(jmp_env, SETJMP_SDWR_ERR);		
	//if((r = SD_wr(SD_WR_OPT_CHK_AFTER, p, SD_PARTAB_SECTOR)) != OK)
	//if((r = SD_wr(SD_WR_OPT_SIMPLEWR, p, SD_PARTAB_SECTOR)) != OK)
		
		

	#if 1
	{//debug
	
		debug.puthex("partition table sector: ", SD_PARTAB_SECTOR);
		debug.puthex("NSECTORS: ", NSECTORS);
		
		//SD_rd(0, p, SD_PARTAB_SECTOR);
		//debug.memdump("", p, 512);		
	}
	#endif
		

	/***************************************************************************
	* boot sector
	***************************************************************************/
	debug.puttxt("**** boot sector ****");

	memset(p->raw, 0, SECTOR_SIZE);	
	
	p->boot_sector.jmp_boot[0] = 0xe9;
	p->boot_sector.jmp_boot[1] = 0x68;
	p->boot_sector.jmp_boot[2] = 0x06;
	memcpy(p->boot_sector.oem_name, "__55H__", 8);
	p->boot_sector.bytes_per_sector = SECTOR_SIZE;
	p->boot_sector.num_fats = 2;
	p->boot_sector.media = 0xf8;
	p->boot_sector.hidden_sectors = 1;
	p->boot_sector.root_cluster = 2;
	p->boot_sector.fsinfo_sector = SD_BOOT_SECTOR;
	p->boot_sector.backup_bootrec = SD_BOOT_BK_SECTOR;
	p->boot_sector.boot_signature = 0x29;
	p->boot_sector.volume_serial = 0x12345678;
	memcpy(p->boot_sector.volume_label, vol_id_v1, SHORTNAMELEN);
	memcpy(p->boot_sector.file_system_type, "FAT32   ", 8);
	p->boot_sector.reserved_sectors = SD_RESERVED_SECTORS;
	p->boot_sector.sectors_per_cluster = SECPCLUSTER;
	p->boot_sector.sectors_per_fat = SECPFAT;
	p->boot_sector.total_sectors = NSECTORS;
	p->boot_sector.signature = 0xaa55;
	
	if(SD_wr(p, SD_BOOT_SECTOR) != OK)
		longjmp(jmp_env, SETJMP_SDWR_ERR);		
	//if((r = SD_wr_intel(SD_WR_OPT_CHK_AFTER, p, SD_BOOT_SECTOR)) != OK)
	//if((r = SD_wr_intel(SD_WR_OPT_SIMPLEWR, p, SD_BOOT_SECTOR)) != OK)
	if(SD_wr(p, SD_BOOT_BK_SECTOR) != OK)
		longjmp(jmp_env, SETJMP_SDWR_ERR);		
	//if((r = SD_wr_intel(SD_WR_OPT_CHK_AFTER, p, SD_BOOT_BK_SECTOR)) != OK)
	//if((r = SD_wr_intel(SD_WR_OPT_SIMPLEWR, p, SD_BOOT_BK_SECTOR)) != OK)

	#if 1
	{//debug
		debug.puthex("boot sector: ", SD_BOOT_SECTOR);
		debug.puthex("boot backup sector: ", SD_BOOT_BK_SECTOR);
		
		debug.puthex("NSECTORS: ", NSECTORS);
		debug.puthex("NCLUSTERS: ", NCLUSTERS);
		debug.puthex("CLUSTER_SIZE: ", CLUSTER_SIZE);
		debug.puthex("SECPCLUSTER: ", SECPCLUSTER);
		debug.puthex("SECPFAT: ", SECPFAT);
		debug.puthex("DATACLUSTERS: ", DATACLUSTERS);

		

		//debug.puthex("sector: ", SD_BOOT_SECTOR);
		//SD_rd_intel(0, p, SD_BOOT_SECTOR);
		//debug.memdump("", p, 512);		
		//debug.puthex("bk sector: ", SD_BOOT_BK_SECTOR);
		//SD_rd_intel(0, p, SD_BOOT_BK_SECTOR);
		//debug.memdump("", p, 512);		
	}
	#endif
	

	/***************************************************************************
	* FSinfo sector
	***************************************************************************/
	debug.puttxt("**** FSinfo sector ****");
	
	memset(p->raw, 0, SECTOR_SIZE);

	p->fsinfo_sector.lead_signature = 0x41615252;
	p->fsinfo_sector.struct_signature = 0x61417272;

	//p->fsinfo_sector.free_count = 0x00075c4b;
	//p->fsinfo_sector.nxt_free = 0x00000004;
	p->fsinfo_sector.free_count = 0xffffffff;
	p->fsinfo_sector.nxt_free = 0xffffffff;
	p->fsinfo_sector.signature = 0xaa550000;

	if(SD_wr(p, SD_BOOTFSINFO_SECTOR) != OK)
		longjmp(jmp_env, SETJMP_SDWR_ERR);		
	//if((r = SD_wr_intel(SD_WR_OPT_CHK_AFTER, p, SD_BOOTFSINFO_SECTOR)) != OK)
	//if((r = SD_wr_intel(SD_WR_OPT_SIMPLEWR, p, SD_BOOTFSINFO_SECTOR)) != OK)
	if(SD_wr(p, SD_BOOTFSINFO_BK_SECTOR) != OK)
		longjmp(jmp_env, SETJMP_SDWR_ERR);		
	//if((r = SD_wr_intel(SD_WR_OPT_CHK_AFTER, p, SD_BOOTFSINFO_BK_SECTOR)) != OK)
	//if((r = SD_wr_intel(SD_WR_OPT_SIMPLEWR, p, SD_BOOTFSINFO_BK_SECTOR)) != OK)

	#if 0
	{//debug
		//debug.puthex("sector: ", SD_BOOTFSINFO_SECTOR);
		//SD_rd_intel(0, p, SD_BOOTFSINFO_SECTOR);
		//debug.memdump("", p, 512);		
		//debug.puthex("bk sector: ", SD_BOOTFSINFO_BK_SECTOR);
		//SD_rd_intel(0, p, SD_BOOTFSINFO_BK_SECTOR);
		//debug.memdump("", p, 512);		
	}
	#endif

	memset(p->raw, 0, SECTOR_SIZE);
	p->fsinfo_sector.signature = 0xaa550000;
	if(SD_wr(p, SD_BOOTFSINFO_SECTOR + 1) != E_EJ_OK)
		longjmp(jmp_env, SETJMP_SDWR_ERR);		
	//if((r = SD_wr_intel(SD_WR_OPT_CHK_AFTER, p, SD_BOOTFSINFO_SECTOR + 1)) != OK)
	//if((r = SD_wr_intel(SD_WR_OPT_SIMPLEWR, p, SD_BOOTFSINFO_SECTOR + 1)) != OK)
	if(SD_wr(p, SD_BOOTFSINFO_BK_SECTOR + 1) != E_EJ_OK)
		longjmp(jmp_env, SETJMP_SDWR_ERR);		
	//if((r = SD_wr_intel(SD_WR_OPT_CHK_AFTER, p, SD_BOOTFSINFO_BK_SECTOR + 1)) != OK)
	//if((r = SD_wr_intel(SD_WR_OPT_SIMPLEWR, p, SD_BOOTFSINFO_BK_SECTOR + 1)) != OK)
	
	#if 0
	{//debug
		//debug.puthex("sector: ", SD_BOOTFSINFO_SECTOR + 1);
		//SD_rd_intel(0, p, SD_BOOTFSINFO_SECTOR + 1);
		//debug.memdump("", p, 512);		
		//debug.puthex("bk sector: ", SD_BOOTFSINFO_BK_SECTOR + 1);
		//SD_rd_intel(0, p, SD_BOOTFSINFO_BK_SECTOR + 1);
		//debug.memdump("", p, 512);		
	}
	#endif
	
	/***************************************************************************
	* clear reserved area: sector: 4,5, 9 -> SD_RESERVED_SECTORS
	***************************************************************************/
	debug.puttxt("**** clear reserved area ****");
	
	memset(p->raw, 0, SECTOR_SIZE);
	for(i = 4; i < SD_RESERVED_SECTORS + 1; i++){
		if(i == 6)
			i = 9;
		//if((r = SD_wr_intel(SD_WR_OPT_CHK_AFTER, p, i)) != OK){
		//if((r = SD_wr_intel(SD_WR_OPT_SIMPLEWR, p, i)) != OK){
		if(SD_wr(p, i) != E_EJ_OK)
			longjmp(jmp_env, SETJMP_SDWR_ERR);		
		#if 0
		{//debug
			//debug.puthex("sector: ", i);
			//SD_rd_intel(0, p, i);
			//debug.memdump("", p, 512);		
		}
		#endif
	}


	/***************************************************************************
	* FAT fill
	***************************************************************************/
	debug.puttxt("**** FAT sectors ****");
	debug.puthex("1st FAT sector : ", SD_1ST_FAT_SECTOR);
	debug.puthex("SD_1ST_DIR_SECTOR : ", SD_1ST_DIR_SECTOR);
	debug.puthex("DIRCLUSTERS: ", DIRCLUSTERS);
	debug.puthex("SD_1ST_DATA_SECTOR : ", SD_1ST_DATA_SECTOR);
	
	
	for(nextcluster = 1, i = 0; i < SECPFAT; i++){
		for(k = 0; k != SECTOR_SIZE / sizeof(int); k++){
			p->fat[k] = nextcluster & 0x0fffffff;
			if((nextcluster & 0x80000000) == 0)
				nextcluster++;
			
			//debug.puthex("nextcluster: ", nextcluster);
			//if((nextcluster >= (2 + DIRCLUSTERS + 1)) && (nextcluster <= ((2 + DIRCLUSTERS + 1) + SYSTEMFILES))){//for dir and SYSTEMFILES
			if(nextcluster == 2 + DIRCLUSTERS + 1){
				p->fat[k] = FAT32_EOC;
				debug.puthex("p->fat[k] = FAT32_EOC: ", k);////FAT_DIR_EOF p->fat[k] = FAT32_EOC: 0000003A
			}
	// 2G cards only !!!!! 4 clusters -> 64k @ 2GB (32 sec/clusters)
			if(nextcluster == DATACLUSTERS + 4){  // !!!! hack, for some reason, we have 4 free clusters by windows...
				nextcluster = 0x80000000;
				p->fat[k] = FAT32_EOC;
				debug.puthex("p->fat[k**] = FAT32_EOC: ", k);////FAT_EOF p->fat[k**] = FAT32_EOC: 00000042
			}
		}
		if(i == 0){
			p->fat[0] = FAT32_MEDIA;
			p->fat[1] = FAT32_FLAGS;
		}
		if(SD_wr(p, i + SD_1ST_FAT_SECTOR) != OK)
			longjmp(jmp_env, SETJMP_SDWR_ERR);		
		//if((r = SD_wr_intel(SD_WR_OPT_CHK_AFTER, p, i + SD_1ST_FAT_SECTOR)) != OK)
		//if((r = SD_wr_intel(SD_WR_OPT_SIMPLEWR, p, i + SD_1ST_FAT_SECTOR)) != OK)
		if(SD_wr(p, i + SD_1ST_FAT_SECTOR + SECPFAT) != OK)
			longjmp(jmp_env, SETJMP_SDWR_ERR);		
		//if((r = SD_wr_intel(SD_WR_OPT_CHK_AFTER, p, i + SD_1ST_FAT_SECTOR + SECPFAT)) != OK)
		//if((r = SD_wr_intel(SD_WR_OPT_SIMPLEWR, p, i + SD_1ST_FAT_SECTOR + SECPFAT)) != OK)
		#if 0
		{//debug
			//debug.puthex("sector: ", i + SD_1ST_FAT_SECTOR);
			//SD_rd_intel(0, p, i + SD_1ST_FAT_SECTOR);
			//debug.memdump("", p, 512);		
			//debug.puthex("bk sector: ", i + SD_1ST_FAT_SECTOR + SECPFAT);
			//SD_rd_intel(0, p, i + SD_1ST_FAT_SECTOR + SECPFAT);
			//debug.memdump("", p, 512);		
		}
		#endif
		WATCHDOG_RESET;

	}

	/***************************************************************************
	* dir entries of ej's
	***************************************************************************/
	debug.puttxt("**** dir sectors : delete mark ****");

	memset(p->raw, 0, SECTOR_SIZE);
	for(k = 0; k < 16; k++){
		p->root_dir[k].dir_name[0] = 0xe5;
	}

	for(j = 0; j < DIRCLUSTERS; j++){
		for(i = 0; i < SECPCLUSTER; i++){	        // fill the remainder of this cluster (2?)
			if(SD_wr(p, i + j * SECPCLUSTER + SD_1ST_DIR_SECTOR) != OK)
				longjmp(jmp_env, SETJMP_SDWR_ERR);		
			//SD_wr_intel(SD_WR_OPT_CHK_AFTER, p, i + j * SECPCLUSTER + SD_1ST_DIR_SECTOR);
			//SD_wr_intel(SD_WR_OPT_SIMPLEWR, p, i + j * SECPCLUSTER + SD_1ST_DIR_SECTOR);

			{//debug
				//debug.puthex("sector: ", i + j * SECPCLUSTER + SD_1ST_DIR_SECTOR);
				//SD_rd_intel(0, p, i + j * SECPCLUSTER + SD_1ST_DIR_SECTOR);
				//debug.memdump("", p, 512);		
			}
			WATCHDOG_RESET;

		}
	}

	/***************************************************************************
	* root dir
	***************************************************************************/
	debug.puttxt("**** root dir ****");
	memset(p->raw, 0, SECTOR_SIZE);

	memcpy(p->root_dir[0].dir_name, vol_id_v1, SHORTNAMELEN);
	p->root_dir[0].dir_attr = ATTR_VOLUME_ID;
	p->root_dir[0].dir_wrttime = 0x1234;
	p->root_dir[0].dir_wrtdate = 0x5678;

	#if 0
	for(k = 1; k < SYSTEMFILES + 1; k++){
		//link error: why? sprintf(p->root_dir[k].dir_name, "__BC__%02dsy", (uint8_t)k-2);
		memcpy(p->root_dir[k].dir_name, "BC_SYS0xBIN", SHORTNAMELEN);//CAPITAL LETTERS!!!
		p->root_dir[k].dir_name[7] = hextab[k & 0x0f];
		//p->root_dir[k].dir_attr = ATTR_HIDDEN | ATTR_SYSTEM;
		p->root_dir[k].dir_attr = ATTR_READ_ONLY;
		p->root_dir[k].dir_ntres = 0x10;//info got from a win formated sd 
		p->root_dir[k].dir_crttimetenth = 0x0e;//info got from a win formated sd
		p->root_dir[k].dir_clust_h = 0;
		p->root_dir[k].dir_clust_l = SD_RESERVED_CLUSTERS + DIRCLUSTERS + k - 1;
		p->root_dir[k].dir_crttime =
		p->root_dir[k].dir_wrttime = 0x784f;
		p->root_dir[k].dir_crtdate =
		p->root_dir[k].dir_wrtdate = 0x378d;
		p->root_dir[k].dir_filesize = CLUSTER_SIZE;
	}
	#else
	for(k = 1; k < 16; k++){
		p->root_dir[k].dir_name[0] = 0xe5;
	}
	#endif

	if(SD_wr(p, SD_1ST_DIR_SECTOR) != E_EJ_OK)
		longjmp(jmp_env, SETJMP_SDWR_ERR);		
	//if((r = SD_wr_intel(SD_WR_OPT_CHK_AFTER, p, SD_1ST_DIR_SECTOR)) != OK)
	//if((r = SD_wr_intel(SD_WR_OPT_SIMPLEWR, p, SD_1ST_DIR_SECTOR)) != OK)
	
	#if 0
	{//debug
		//debug.puthex("sector: ", SD_1ST_DIR_SECTOR);
		//SD_rd_intel(0, p, SD_1ST_DIR_SECTOR);
		//debug.memdump("", p, 512);		

	}
	#endif

	debug.puttxt("formating done");
	return filesys_result = OK;
}


