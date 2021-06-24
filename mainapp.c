#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <windows.h>

//This block of codes was made by M. Raihan Azhari//
//------------------------------------------------
struct amalan{

	//variabel amanalan ibadah harian
	int tahajud;
	int dhuha;
	int wajib;
	int tilawah;
	int tahfidz;
	
	struct amalan *next;
};

typedef struct User{
	
	//variabel data user pengguna aplikasi (hanya 1 user)
	char nama[30];
	int target_status;
	int target_tahajud, target_dhuha, target_wajib;
	int target_tilawah, target_tahfidz;
	int day;
	
	struct amalan *data;
}User;

typedef struct amalan Amalan;
typedef Amalan *Amalanptr;

struct keluarga{ //struct untuk anggota keluarga waris

  //variabel anggota keluarga
	int suami;
	int istri;
	int anaklk;
	int anakpr;
	int sdrlk;
	int sdrpr;
	int bapak;
	int ibu;
	int cuculk;
	int cucupr;
};

//end of codes block
//----------------------------------------------------

void input_data(Amalanptr *sptr);
void input_menu();
void printAmalan(Amalanptr current, int day_removed[50], User *userptr);
void print_evaluasi(Amalanptr current, User *userptr , int day_removed[50]);
void removeptr (Amalanptr *startPtr, int day, int day_removed[50]);
int file_user_read (User *userptr, int day_removed[50]);
int file_user_write (User *userptr, int day_removed[50]);
int file_amalan_write (Amalanptr current, int day_removed[50]);
int file_amalan_read (Amalanptr *sptr, int day_removed[50], int i, int *posisi);
void welcome(User *userptr);
void help_mutabaah();
int file_removed_write(int day[50]);
int file_removed_read(int day[50]);
int menuZakat();
void zakatPenghasilan();
void zakatMaal();
void list_zakatMaal();
void petunjuk_zakatPenghasilan();
void petunjuk_zakatMaal();
int jumlah(int x[], int n, int i);
int zakat(int * x, int y);
int menuWaris();
void perhitungan(int harta, int warisan[10], struct keluarga keluarga, int keturunan, int saudara, int orangtua); //algoritma perhitungan harta warisan
void list(struct keluarga keluarga); //list anggota keluarga yang mendapatkan warisan
void display(int warisan[10]); //menampilkan hasil perhitungan harta waris

int main(){

	FILE *fptr;
	
	int menu, day, i, j, id_input,id, mutabaah, status, login, login_status, file_status, posisi;
	int day_removed[50] = {}; //array untuk mengetahui hari yang telah di remove
	
	//set variabel ke 0 untuk mencegah adanya garbage value
	menu = 0;
	mutabaah = 0;
	id = 0;
	//struct user dalam fungsi ini dapat dipanggil melalui pointer *userptr
	User *userptr;
	userptr = (User*) calloc(1, 2 * sizeof(User));
	/*Tadinya kami ingin membuat multi-user berdasarka idnya, namun karena kesulitan pada file handling maka
		kami membuat 1 user saja, namun tetap menggunakan pointer struct*/
  
  	//start pointer dibuat NULL
	Amalanptr startptr = NULL;
	
	//pembacaan file
	file_status = file_user_read(userptr, day_removed); // file_user_read merupakan file data user
	file_removed_read(day_removed); //file_removed_read merupakan file untuk membaca hari hari yang telah diremove
	
	//posisi untuk membaca file di set ke-0 untuk mencegah garbage value
	posisi = 0;
	
  	//jika file_amalan_read masih belum dibuat
	if((userptr + id)->day == 0){
		file_amalan_read(&startptr, day_removed, 0, &posisi);
	}
	
	//jika file_amalan_read sudah dibuat
	else{
	//loop dijalankan sebanyak data hari yang telah diinput oleh user
		for(i = 0; i < (userptr + id)->day - 1; i++){
			file_amalan_read(&startptr, day_removed, i, &posisi);
		}
		//loop terakhir mempassing nilai -1 untuk menutup file
		file_amalan_read(&startptr, day_removed, -1, &posisi);
	}
	
	//link dari userptr dilink ke alamat awal dari link list amalan (startptr)
	(userptr + id)->data = startptr; 
	
	//loop akan terus berjalan hingga user memasukan angka -1
	while (menu != -1){
		welcome(userptr);
		login_status = 1;
		printf("\nMasukan angka: ");
		scanf("%d", &menu);
		system("cls");
		//mutabaah diset ke angka 0 untuk mereset keadaan switch case
		mutabaah = 0;

		switch (menu){
			case 1: 
			//---------------------------------------------------
			//case 1 (Mutaba'ah Yaumiah) was made by M. Raihan Azhari
    		//loop akan terus berjalan sampai user memasukan angka -1
				while(mutabaah != -1){
					help_mutabaah();
					printf("\nMasukan pilihan metode mutabaah: ");
					scanf("%d", &mutabaah);
					system("cls");
					switch (mutabaah){
					case 1: 
            			// user diminta untuk menginput identitas dan target ibadah
            			//target_status bernialai 1 menandakan user sudah menginput data identitas dan target
						(userptr + id)->target_status = 1; 
						printf("\nMasukan Nama : ");
						scanf("%s", &(userptr + id)->nama);
						printf("\n Masukan Target Rakaat Tahajud: ");
						scanf("%d", &(userptr + id)->target_tahajud);
						printf("\n Masukan Target Rakaat Dhuha: ");
						scanf("%d", &(userptr + id)->target_dhuha);
						(userptr + id)->target_wajib = 5; //target sholat wajib di set sebagai 5 karena merupakan ibadah wajib
						printf("\n Masukan Target Halaman Tilawah: ");
						scanf("%d", &(userptr + id)->target_tilawah);
						printf("\n Masukan Target Ayat Tahfidz: ");
						scanf("%d", &(userptr + id)->target_tahfidz);
						printf("\n\n");
						system("pause");
						system("cls");
						break;
						
					case 2:
           				 //case 2 user diminta untuk menginput amalan ibadah sesuai hari yang diinginkan		
						//jika target status != 1 maka error handling akan berjalan dan user diharapkan input case 1 terlebih dahulu				
						if((userptr + id)->target_status != 1){
							printf("\nHarap masukan target terlebih dahulu\n\n");
							system("pause");
							system("cls");
							break;
						}
						
						printf("\nAssalamualaikum %s", (userptr + id)->nama);
						printf("\nMasukan jumlah hari yang akan diinput: ");
						scanf("%d", &day);
           				 //user meminta input amalan sebanyak hari yang diinginkan
						for(i = 0; i < day; i++){
							printf("\n\nAmalan hari ke-%d", (userptr->day) + i + 1);
               			 //input_data dipassing address dari start pointer sebagai pointer awal dalam link listed
							input_data(&startptr);
						}
						//struct dari userptr dihubungkan ke starting pointer link listed amalan harian
						(userptr + id)->data = startptr; 
						system("cls");
						printf("\n\ninput berhasil !\n");
						printAmalan((userptr + id)->data, day_removed, userptr);
						printf("\n\n");
						system("pause");
						system("cls");
						break;
						
					case 3:
						//case 3 user ditampilkan hasil evaluasi ibadah harian yang telah diinput
						//error handling jika user belum memasukan identitas dan target
						if((userptr + id)->target_status != 1){
							printf("\nHarap masukan target terlebih dahulu\n\n");
							system("pause");
							system("cls");
							break;
						}
						//mencetak detail amalan harian yang dilakukan oleh user
						printAmalan((userptr + id)->data, day_removed, userptr);
						printf("\n\n");
						system("pause");
						system("cls");
						//mencetak rekap amalan harian yang dilakukan oleh user beserta evaluasinya
						print_evaluasi((userptr + id)->data, userptr, day_removed);
						printf("\n\n");
						system("pause");
						system("cls"); 
						break;
						
					case 4:
						if((userptr + id)->target_status != 1){
							printf("\nHarap masukan target terlebih dahulu\n\n");
							system("pause");
							system("cls");
							break;
						}
						printf("Pilih hari yang akan dihapus: ");
						scanf("%d", &day);
						day--;
						removeptr (&startptr, day, day_removed);
						day_removed[day] = 1;
						printf("\nData hari ke-%d berhasil dihapus\n\n", day+1);
						system("pause");
						system("cls");
						
				}	
			}
			//end of codes block
			//---------------------------------------------------------
			
			case 2: 
				menuZakat();
				break;
			case 3: 
			//---------------------------------------------------
			//case 3 (Harta Waris) was made by Fikri Afif Musyaffa
				menuWaris();
				break;

		}
	}
	
	file_user_write(userptr, day_removed);
	file_amalan_write((userptr + id)->data, day_removed);
	file_removed_write(day_removed);
	
  return 0;
}

//--------------------------------------------------------
//This function was made by M. Raihan Azhari
void input_data(Amalanptr *sptr){
	
//	int sks_var, kode_var, bobot_var, status_var, condition;
//	char nilai_var;
	
	Amalanptr currentptr;
	Amalanptr newptr;
	Amalanptr prevptr;
	
	newptr = malloc(sizeof(Amalan));
	
	//masukin kode input variabel apa aja disini
	
	printf("\nMasukan amalan");
	printf("\nRakaat Tahajud: ");
	scanf("%d", &newptr->tahajud);
	printf("Rakaat Dhuha: ");
	scanf("%d", &newptr->dhuha);
	printf("Banyak Sholat Wajib yang Dikerjakan: ");
	scanf("%d", &newptr->wajib);
	printf("Jumlah Halaman Tilawah: ");
	scanf("%d", &newptr->tilawah);
	printf("Jumlah Ayat Tahfidz: ");
	scanf("%d", &newptr->tahfidz );

	newptr->next = NULL;
	
	prevptr = NULL;
	
	currentptr = *sptr;
	
	while(currentptr != NULL){
		prevptr = currentptr;
		currentptr = currentptr->next;
	}
	
	if (prevptr == NULL){
		newptr->next = *sptr;
		*sptr = newptr;
	}
	
	else{
		prevptr->next = newptr;
		newptr->next = currentptr;
	}	
}
//end of codes block
//----------------------------------------------------


//---------------------------------------------------
//This function was made by M. Raihan Azhari
void printAmalan(Amalanptr current, int day_removed[50], User *userptr){
	
	//ini masih yang matkul blm gw ganti hehe
	int counter;
	counter = 0;
	
	while(current != NULL){
		
		if(day_removed[counter] == 1){
			printf("\n\nRekap ibadah hari ke-%d telah dihapus \n", counter + 1);
		}
		else{
			
		#pragma omp parallel
		{
			int tid;
		
			tid = omp_get_thread_num();
			
			#pragma omp single
			{
				printf("\n\nRekap ibadah hari ke-%d:", counter + 1);	
			}
			
			#pragma omp taskwait
			
			if(tid == 0){
				printf("\nTahajud : %d Rakaat",current->tahajud);
				printf("\nDhuha : %d Rakaat",current->dhuha);
				printf("\nWajib : %d Waktu",current->wajib);
			
			}
		
			if (tid == 1){				
			printf("\nTilawah : %d Halaman",current->tilawah);
			printf("\nTahfidz : %d Ayat",current->tahfidz);
			}
			
			#pragma omp taskwait
		
		}
	}
	//test
		current = current->next;
		counter++;
	}
	
	userptr->day = counter;
		
}

//end of codes block
//------------------------------------------------------------


//-------------------------------------------------------------
//this function was made by M. Raihan Azhari
void print_evaluasi(Amalanptr current, User *userptr , int day_removed[50]){
	
	int counter, i;
	int jumlah_tahajud, jumlah_dhuha, jumlah_wajib, jumlah_tilawah, jumlah_tahfidz, avg, tugas, step;
	
	jumlah_tahajud = 0;
	jumlah_dhuha = 0;
	jumlah_wajib = 0;
	jumlah_tilawah = 0;
	jumlah_tahfidz = 0;
	counter = 0;
	

	while (current != NULL){
		
		jumlah_tahajud += current->tahajud;
		jumlah_dhuha += current->dhuha;
		jumlah_wajib += current->wajib;
		jumlah_tilawah += current->tilawah;
		jumlah_tahfidz += current->tahfidz;
		counter ++;
		
		current = current->next;
	}
	
	userptr->day = counter;
	
	if(counter == 0){
		printf("Data masih kosong");
	}
	
	else{
		printf("\n Evaluasi ibadah harian selama %d hari: ", counter);
		tugas = 0;
		step = 0;
		
		#pragma omp parallel private(tugas, step)
		{
			#pragma omp master
			{
				for(i = step; i < 5; i++){
					tugas = i;
					step = i;
					#pragma omp task
					{
						if (tugas == 0 && userptr->target_tahajud != 0){
							float rata_tahajud = jumlah_tahajud / (float)counter;
							float result_tahajud = rata_tahajud /(float) userptr->target_tahajud;
							#pragma omp critical
							{
								printf("\n\n~~Tahajud~~ ");
								printf("\nRata-rata Rakaat setiap harinya : %.2f", rata_tahajud);
								printf("\nPersen ketercapaian target: %.2f %", result_tahajud * 100);
							}
						}
						
						if(tugas == 1 && userptr->target_dhuha != 0){
							float rata_dhuha = jumlah_dhuha / (float)counter;
							float result_dhuha = rata_dhuha /(float)userptr->target_dhuha;
							#pragma omp critical
							{
								printf("\n\n~~Dhuha~~ ");
								printf("\nRata-rata Rakaat setiap harinya : %.2f", rata_dhuha);
								printf("\nPersen ketercapaian target: %.2f %", result_dhuha * 100);
							}
						}
						
						if(tugas == 2 && userptr->target_wajib != 0){
							float rata_wajib = jumlah_wajib / (float)counter;
							float result_wajib = rata_wajib /(float)userptr->target_wajib;
							#pragma omp critical
							{
								printf("\n\n~~Sholat wajib 5 waktu~~ ");
								printf("\nRata-rata Rakaat setiap harinya : %.2f", rata_wajib);
								printf("\nPersen ketercapaian target: %.2f %", result_wajib * 100);
							}
						}
						if(tugas == 3 && userptr->target_tilawah != 0){
							float rata_tilawah = jumlah_tilawah / (float)counter;
							float result_tilawah = rata_tilawah /(float)userptr->target_tilawah;
							#pragma omp critical
							{
								printf("\n\n~~Tilawah~~ ");
								printf("\nRata-rata Halaman setiap harinya : %.2f", rata_tilawah);
								printf("\nPersen ketercapaian target: %.2f %", result_tilawah * 100);
							}
						}
						if(tugas == 4 && userptr->target_tilawah != 0){
							float rata_tahfidz = jumlah_tahfidz / (float)counter;
							float result_tahfidz = rata_tahfidz /(float)userptr->target_tahfidz;
							#pragma omp critical
							{
									printf("\n\n~~Tahfidz~~ ");
									printf("\nRata-rata Halaman setiap harinya : %.2f", rata_tahfidz);
									printf("\nPersen ketercapaian target: %.2f %", result_tahfidz * 100);
		
							}
						}						
					}
				}
			}
			#pragma omp taskwait
		}
	}
}

//-----------------------------------------------------------------------------------


//-----------------------------------------------
//Muhammad Raihan Azhari
void removeptr (Amalanptr *startPtr, int day, int day_removed[50]){
	
	Amalanptr prevPtr;
	Amalanptr tempPtr;
	Amalanptr currentPtr;
	
	int i, hari;
	
	day++;
	
	if ( day == 0) {
 		tempPtr = *startPtr;
		*startPtr = ( *startPtr )->next; 
 		free( tempPtr ); 
 	}
 	else {
 		prevPtr = *startPtr;
 		currentPtr = ( *startPtr )->next;
 
 		for(i = 1; i < day; i++){
 			
 			if(day_removed[i]== 1){
 				continue;
			 }
 			
 			if (i == day) {
		 		tempPtr = currentPtr;
		 		prevPtr->next = currentPtr->next;
		 		free( tempPtr );
		 	} 
 			prevPtr = currentPtr; 
 			currentPtr = currentPtr->next;
		
		 	if(currentPtr == NULL) {
			 	break;
			 }
 		}
 	}
}
//------------------------------------------------------

//---------------------------------------------------
//Fikri Afif Musyaffa 
int file_user_read (User *userptr, int day_removed[50]){
	
	FILE *fptr;
	
	fptr = fopen("userMuslim.txt", "r");
	
	if(fptr == NULL){
	//	printf("\nFile user belmu dibuat");
		fclose(fptr);
		fptr = fopen("userMuslim.txt", "w");
		fclose(fptr);
		return 0;
	}
	else{	
		fseek(fptr, 0, SEEK_SET);
	//	printf("\nFile sudah dibuat");
		fscanf(fptr, "\n%s", &userptr->nama);
		fscanf(fptr, "\n%d", &userptr->target_status);
		fscanf(fptr, "\n%d", &userptr->target_tahajud);
		fscanf(fptr, "\n%d", &userptr->target_dhuha);
		fscanf(fptr, "\n%d", &userptr->target_wajib);
		fscanf(fptr, "\n%d", &userptr->target_tilawah);
		fscanf(fptr, "\n%d", &userptr->target_tahfidz);
		fscanf(fptr, "\n%d", &userptr->day);
	}
	fclose(fptr);
	return 1;
}
//--------------------------------------------------


//---------------------------------------------------
//Fikri Afif Musyaffa
int file_user_write (User *userptr, int day_removed[50]){
	
	FILE *fptr;
	
	fptr = fopen("userMuslim.txt", "w");
	
	fprintf(fptr, "\n%s", userptr->nama);
	fprintf(fptr, "\n%d", userptr->target_status);
	fprintf(fptr, "\n%d", userptr->target_tahajud);
	fprintf(fptr, "\n%d", userptr->target_dhuha);
	fprintf(fptr, "\n%d", userptr->target_wajib);
	fprintf(fptr, "\n%d", userptr->target_tilawah);
	fprintf(fptr, "\n%d", userptr->target_tahfidz);
	fprintf(fptr, "\n%d", userptr->day);
	fclose(fptr);
}

//---------------------------------------------------


//---------------------------------------------------
//M. Raihan Azhari
int file_amalan_read (Amalanptr *sptr, int day_removed[50], int i, int *posisi){
	
	FILE *fptr;
	
	
	int tahajud_var, dhuha_var, wajib_var, tilawah_var, tahfidz_var;
		
	Amalanptr currentptr;
	Amalanptr newptr;
	Amalanptr prevptr;
	
	newptr = malloc(sizeof(Amalan));
	
	if(i == 0){
		fptr = fopen("amalan.txt", "r");
	}
	
	if(fptr == NULL){
	//	printf("\nFile amalan belmu dibuat");
		fclose(fptr);
		fptr = fopen("userMuslim.txt", "w");
		fclose(fptr);
		return 0;
	}
	
	else{
		if(i == 0){
			fseek(fptr, 0, SEEK_SET);
		}
		
		else{
			fseek(fptr,  0 , SEEK_CUR);
		}
	//	printf("\nFile sudah dibuat");
		
		
		fscanf(fptr,"\n%d",&newptr->tahajud);
		fscanf(fptr,"\n%d",&newptr->dhuha);
		fscanf(fptr,"\n%d",&newptr->wajib);
		fscanf(fptr,"\n%d",&newptr->tilawah);
		fscanf(fptr,"\n%d",&newptr->tahfidz);
		
		newptr->next = NULL;
	
		prevptr = NULL;
	
		currentptr = *sptr;
	
		while(currentptr != NULL){
			prevptr = currentptr;
			currentptr = currentptr->next;
		}
	
		if (prevptr == NULL){
			newptr->next = *sptr;
			*sptr = newptr;
		}
	
		else{
			prevptr->next = newptr;
			newptr->next = currentptr;
		}
		
		if(i == -1){
			fclose(fptr);
		}	
		return 1;
	}
}
//--------------------------------------------------


//---------------------------------------------------
//M. Raihan Azhari
int file_amalan_write (Amalanptr current, int day_removed[50]){
	
	FILE *fptr;
	
	fptr = fopen("amalan.txt", "w");
	
	while (current != NULL){
		
		fprintf(fptr,"\n%d",current->tahajud);
		fprintf(fptr,"\n%d",current->dhuha);
		fprintf(fptr,"\n%d",current->wajib);
		fprintf(fptr,"\n%d",current->tilawah);
		fprintf(fptr,"\n%d",current->tahfidz);
		
		current = current->next;
	}
	
	fclose(fptr);
	
}
//---------------------------------------------------


void welcome(User *userptr){
	
	int i, tid;
	
	
	#pragma omp for
	for(i = 0; i < 60; i++){
		printf("-");
	}
	
	#pragma omp barrier
	printf("\n\t\t\t Mu'min Pro\n");
	#pragma omp for
	for(i = 0; i < 60 ;i++){
		printf("-");
	}
	
	#pragma omp barrier
	
	printf("\nAssalamu'alaikum %s", (userptr)->nama);
	printf("\n\nMode Menu Mutabaah: ");
	printf("\n1. Mutaba'ah Yaumiah (Evaluasi Ibadah Harian)");
	printf("\n2. untuk Kalkulator Perhitungan Zakat");
	printf("\n3. untuk Kalkulator Perhitungan Waris\n\n" );
	printf("\n-1 Untuk keluar program");
}

void help_mutabaah(){
	
	printf("\n1. Input Target Mutabaah");
	printf("\n2. Input Ibadah Harian");
	printf("\n3. Lihat Evaluasi Ibadah Harian");
	printf("\n4. Menghapus Ibadah Harian");
	printf("\n-1 Untuk keluar program");
	
}

int file_removed_write(int day[50]){
	
	FILE *fptr;
	
	fptr = fopen("removeday.txt", "w");
	
	int i;
	
	for(i = 0; i < 50; i++){
		fprintf(fptr, "\n%d", day[i]);
	}
	
	fclose(fptr);
	
}


int file_removed_read(int day[50]){
	
	FILE *fptr;
	
	fptr = fopen("removeday.txt", "r");
	
	if (fptr == NULL){
		fclose(fptr);
		fptr = fopen("removeday.txt", "w");
		fclose (fptr);
		return 0;
	}
	
	else{
		int i;
		
		for(i = 0 ; i < 50; i++){
			fscanf(fptr, "\n%d", &day[i]);
		}
		return 1;
	}
	
}

//This block of codes made by Muhammad Taqiy Nur Furqon// 
//----------------------------------------------------

//function untuk menampilkan menu perhitungan zakat
int menuZakat() {
	int pilihan;
	int pilihan2;
	
	while(pilihan != -1) {
		system("cls");
		printf("\t\t\t\tKalkulator Zakat\n");
		printf("\n1. Zakat Penghasilan\n");
		printf("2. Zakat Maal\n");
		printf("3. Petunjuk Penggunaan\n");
		printf("-1. Kembali ke menu utama\n");
		printf("\n>>>");
		scanf("%d", &pilihan);
		switch (pilihan) {
			case 1	: 	system("cls");
						zakatPenghasilan();
						system("pause");
						break;
			case 2	: 	system("cls");
						zakatMaal();
						system("pause");
						break;
			case 3	: 	while (pilihan2 != 3) {
							system("cls");
							printf("\t\t\t\tPetunjuk Kalkulator Zakat\n");
							printf("\n1. Zakat Penghasilan\n");
							printf("2. Zakat Maal\n");
							printf("3. Kembali ke menu zakat\n");
							printf(">>>");
							scanf("%d", &pilihan2);
							switch (pilihan2) {
								case 1		:	system ("cls");
												petunjuk_zakatPenghasilan();
												system("pause");
												break;
								case 2		:	system ("cls");
												petunjuk_zakatMaal();
												system("pause");
												break;
								default		: 	printf("Input tidak sesuai\n");
												system("pause");
												break;
							}
						} 
						break;
			default	:	printf("Input tidak sesuai");
						system("cls");
						break;
		}
	}
}

//function untuk menghitung zakat penghasilan 
void zakatPenghasilan() {
	int penghasilan[2], i;
	int pengeluaran;
	int penghasilanBersih = 0;
	int hargaBeras;
	int nisabZakat;
	int jumlahZakat;
	
	printf("\t\t\tZakat Penghasilan\n\n");
	printf("Penghasilan/gaji per bulan:\nRp ");
	scanf("%d", &penghasilan[0]);
	printf("Penghasilan lain-lain per bulan:\nRp ");
	scanf("%d", &penghasilan[1]);
	printf("Hutang/Cicilan untuk Kebutuhan Pokok*\n ");
	printf("Kebutuhan pokok termasuk kebutuhan sandang, pangan, papan, pendidikan, kesehatan dan alat transportasi primer.\nRp ");
	scanf("%d", &pengeluaran);
	
	penghasilanBersih = jumlah(penghasilan, 2, 0) - pengeluaran;
	
	printf("\nJumlah Penghasilan per bulan:\n");
	printf("Rp %d", penghasilanBersih);
	
	printf("\n\t\t\tNisab Zakat Penghasilan\n");
	printf("Nisab adalah syarat jumlah minimum (ambang batas) harta yang dapat dikategorikan sebagai harta wajib zakat.\n"); 
	printf("Untuk penghasilan yang diwajibkan zakat adalah penghasilan yang berada diatas nisab.\n");
	printf("Nisab Zakat Penghasilan adalah setara 522 kg beras normal.\n");
	printf("\n Masukkan harga beras saat ini (per kg)\nRp ");
	scanf("%d", &hargaBeras);
	nisabZakat = hargaBeras * 522;
	printf("\nBesarnya Nisab Zakat Penghasilan per Bulan\n");
	printf("Rp %d", nisabZakat);
	 
	if (penghasilanBersih >= nisabZakat) {
		printf("\n\nAnda diwajibkan membayar zakat\n");
		printf("Jumlah yang harus dibayarkan adalah:\n");
		zakat(&jumlahZakat, penghasilanBersih);
		printf("Rp %d", jumlahZakat);
	}
	else
		printf("\nAnda tidak diwajibkan membayar zakat\n");
}
//function untuk menghitung zakat harta
void zakatMaal() {
	int hartaTersimpan[9], i;
	int harta = 0;
	int hutangTempo;
	int hartaTerhitung;
	int hargaEmas;
	int nisabZakat;
	int jumlahZakat;
	
	list_zakatMaal();//memanggil function list_zakatMaal
	printf("\nMasukkan jumlah harta berikut secara berurutan:\n");
	
	for(i = 0; i < 9; i++) {
		printf("%d. Rp ", i + 1);
		scanf("%d", &hartaTersimpan[i]);
	}
	
	printf("\nHutang jatuh tempo saat membayar kewajiban zakat:\nRp ");
	scanf("%d", &hutangTempo);
	
	hartaTerhitung = jumlah(hartaTersimpan, 9, 0) - hutangTempo;
	
	system("cls");
	//Menampilkan hasil input hartaTersimpan
	list_zakatMaal();
	#pragma omp for
	for(i = 0; i < 9; i++) {
		printf("%d. Rp %d\n", i + 1, hartaTersimpan[i]);
	}
	

	printf("Jumlah harta yang dihitung zakatnya:\n");
	printf("Rp %d\n", hartaTerhitung);
	
	printf("\n Masukkan harga emas saat ini (dalam gram)\nRp ");
	scanf("%d", &hargaEmas);
	nisabZakat = hargaEmas * 85;
	printf("\nBesarnya Nisab Zakat Penghasilan per Bulan\n");
	printf("Rp %d\n", nisabZakat);
	 
	if (hartaTerhitung >= nisabZakat) {
		printf("\nAnda diwajibkan membayar zakat\n");
		printf("Jumlah yang harus dibayarkan adalah:\n");
		zakat(&jumlahZakat, hartaTerhitung);
		printf("Rp %d", jumlahZakat);
	}
	else
		printf("\nAnda tidak diwajibkan membayar zakat\n");
}
//function untuk menghitung jumlah dari suatu array
int jumlah(int x[], int n, int i) {
	int sum;
	
    #pragma omp for
    for(i = 0; i < n; i++) {
    	sum = sum + x[i];
	}
	
	return sum;
}
//function dasar untuk menghitung besar zakat
int zakat(int * x, int y) {
	* x = y * 0.025;
}
//function untuk menampilkan harta yang perlu diinput untuk menghitung zakat maal
void list_zakatMaal() {
	printf("\t\t\tZakat Maal\n\n");
	printf("1. Harta dalam bentuk Tabungan/Giro/Deposito\n");
	printf("2. Harta dalam bentuk logam mulia\n");
	printf("3. Harta dalam bentuk surat berharga\n");
	printf("4. Harta dalam bentuk Properti\n");
	printf("5. Harta dalam bentuk kendaraan\n");
	printf("6. Harta dalam bentuk koleksi seni & barang antik\n");
	printf("7. Harta dalam bentuk stok barang dagangan\n");
	printf("8. Harta dalam bentuk lainnya\n");
	printf("9. Harta dalam bentuk Piutang Lancar\n\n");
}
//function untuk menampilkan petunjuk zakat penghasilan
void petunjuk_zakatPenghasilan() {
	printf("\t\t\tPetunjuk Zakat Penghasilan");
	printf("1. Program ini akan menghitung jumlah zakat penghasilan.\n");
	printf("2. Untuk memulai program ini, masukkan angka 1 pada menu zakat.\n");
	printf("3. Anda akan diminta untuk memasukkan penghasilan bulanan Anda.\n");
	printf("4. Anda juga perlu memasukkan penghasilan bulanan lainnya.\n");
	printf("5. Masukkan pengeluaran Anda per bulan dari kebutuhan pokok.\n");
	printf("6. Yang dimaksud Kebutuhan Pokok adalah kebutuhan sandang, pangan, papan,\n");
	printf("   pendidikan, kesehatan dan alat transportasi primer.\n");
	printf("7. Program akan menampilkan penghasilan bersih Anda.\n");
	printf("8. Masukkan harga beras sebagai nisab zakat penghasilan.\n");
	printf("9. Program akan menununjukkan apakah Anda perlu membayar zakat.\n");
	printf("   Jika iya, maka program akan menampilkan jumlah zakat yang harus dibayarkan.\n\n");
}
//function untuk menampilkan petunjuk zakat harta (maal)
void petunjuk_zakatMaal() {
	printf("\t\t\tPetunjuk Zakat Maal\n");
	printf("1. Program ini akan menghitung jumlah zakat maal.\n");
	printf("2. Untuk memulai program ini, masukkan angka 2 pada menu zakat.\n");
	printf("3. Anda akan diminta untuk memasukkan harta yang Anda miliki sebagai berikut:\n");
	printf("\ta. Harta dalam bentuk Tabungan/Giro/Deposito\n");
	printf("\tb. Harta dalam bentuk logam mulia\n");
	printf("\tc. Harta dalam bentuk surat berharga\n");
	printf("\t   Surat Berharga antara lain nilai tunai dari Reksadana, Saham, Obligasi, Unit Link, dll.\n");
	printf("\td. Harta dalam bentuk Properti\n");
	printf("\t   Rumah (properti) yang digunakan sehari-hari, TIDAK DIKENAKAN ZAKAT.\n");
	printf("\te. Harta dalam bentuk kendaraan\n");
	printf("\t   Kendaraan yang digunakan sehari-hari, TIDAK DIKENAKAN ZAKAT.\n");
	printf("\tf. Harta dalam bentuk koleksi seni & barang antik\n");
	printf("\t   Nilai Koleksi dapat ditaksir sendiri, bila dimungkinkan dapat dibantu kurator seni.\n");
	printf("\tg. Harta dalam bentuk stok barang dagangan\n");
	printf("\th. Harta dalam bentuk lainnya\n");
	printf("\ti. Harta dalam bentuk Piutang Lancar\n\n");
	printf("4. Anda akan diminta untuk memasukkan hutang yang jatuh temponya saat kewajiban membayar zakat\n");
	printf("   Contoh bagi pedagang yang harus melunasi cicilan hutang atas barang yang diperdagangkan.\n");
	printf("5. Masukkan harga emas sebagai nisab zakat maal.\n");
	printf("6. Program akan menununjukkan apakah Anda perlu membayar zakat.\n");
	printf("   Jika iya, maka program akan menampilkan jumlah zakat yang harus dibayarkan.\n\n");
}

//end of codes block
//--------------------------------------------------------


//--------------------------------------------------------
//This function was made by Fikri Afif Musyaffa
int menuWaris(){
	
	struct keluarga keluarga; //mendefinisikan struct keluarga
	int warisan[10]; //isi harta warisan yang diterima masing-masing anggota keluarga
	int harta, hutang; //total harta yang diwarisi
	int pewaris, keturunan, saudara, orangtua, pilih; //variabel cek kondisi 

    printf("Perhitungan Waris\n");
    printf("\nMasukkan total harta yang ditinggalkan Pewaris : ");
    scanf("%d", &harta); //harta awal pewaris
    printf("Masukkan total hutang yang ditinggalkan Pewaris : ");
	scanf("%d", &hutang); //jika pewaris memiliki hutang yang belum terbayar
    harta = harta - hutang; //harta akhir pewaris setelah hutang terbayarkan
    printf("\nTotal harta : Rp. %d\n\n", harta);
    system("pause");
    system("cls");
    printf("Jika pewaris laki-laki, masukkan angka 1");
    printf("\nJika pewaris perempuan, masukkan angka 2");
    printf("\nMasukkan angka : ");
    scanf("%d", &pewaris);
        
        while(pewaris != 1 || pewaris != 2){ //error handling menentukan istri dan suami
          if(pewaris == 1){ //jika pewaris adalah laki-laki
            keluarga.suami = 0; //laki-laki yang meninggal, maka dia tidak mendapatkan harta
            printf("\n\nApakah pewaris memiliki istri yang masih hidup?");
            printf("\nJika iya, masukkan angka 1");
            printf("\nJika tidak, masukkan angka 0");
            printf("\n\nMasukkan angka : ");
            scanf("%d", &keluarga.istri);
            if(keluarga.istri == 1){
              system("cls");
              printf("Masukkan jumlah istri yang masih hidup : "); //kemungkinan istri lebih dari 1
              scanf("%d", &keluarga.istri);
              break;
            }
            else if(keluarga.istri == 0){ //jika istrinya sudah meninggal lebih dulu
              break;
            }
            else{
              printf("\nMasukkan angka dengan benar !\n\n"); //error handling pewaris laki-laki
            }
          }
          else if(pewaris == 2){ //jika pewaris adalah perempuan
            keluarga.istri = 0; //perempuan yang meninggal, maka dia tidak mendapatkan harta
            printf("Apakah pewaris memiliki suami yang masih hidup?");
            printf("\nJika iya, masukkan angka 1");
            printf("\nJika tidak, masukkan angka 0");
            printf("\n\nMasukkan angka : ");
            scanf("%d", &keluarga.suami);
            if(keluarga.suami == 1 || keluarga.suami == 0){ //meminta data apakah suami pewaris masih hidup atau sudah meninggal
              break;
            }
            else{
              printf("\nMasukkan angka dengan benar !\n\n"); //error handling pewaris perempuan
            }
          }
          else{
            printf("Masukkan angka dengan benar ! : "); //error handling pewaris perempuan
            scanf("%d", &pewaris);
          }
        }
        system("cls");
        printf("Apakah pewaris memiliki keturunan yang masih hidup?");
        printf("\nJika iya, masukkan angka 1");
        printf("\nJika tidak, masukkan angka 0");
        printf("\n\nMasukkan angka : ");
        scanf("%d", &keturunan); //cek apakah pewaris memiliki keturunan yang masih hidup
        while(keturunan != 1 || keturunan != 0){ //error handling untuk anak dan cucu
          if(keturunan == 1){ //meminta data anak dan cucu jika ada
            system("cls");
            printf("Masukkan jumlah anak laki-laki : ");
            scanf("%d", &keluarga.anaklk);
            printf("Masukkan jumlah anak perempuan : ");
            scanf("%d", &keluarga.anakpr);
            printf("Masukkan jumlah cucu laki-laki : ");
            scanf("%d", &keluarga.cuculk);
            printf("Masukkan jumlah cucu perempuan : ");
            scanf("%d", &keluarga.cucupr);
            break;
          }
          else if(keturunan == 0){ //mengosongkan semua variabel anak dan cucu
            keluarga.anaklk = 0;
            keluarga.anakpr = 0;
            keluarga.cuculk = 0;
            keluarga.cucupr = 0;
            break;
          }
          else{
            printf("Masukkan angka dengan benar ! : "); //error handling untuk anak dan cucu
            scanf("%d", &keturunan);
          }
        }
        system("cls");
        printf("Apakah pewaris memiliki saudara/i yang masih hidup?");
        printf("\nJika iya, masukkan angka 1");
        printf("\nJika tidak, masukkan angka 0");
        printf("\n\nMasukkan angka : ");
        scanf("%d", &saudara); //cek apakah pewaris memilki saudara kandung yang masih hidup
        while(saudara != 1 || saudara != 0){ //error handling menentukan saudara
          if(saudara == 1){ //meminta data jumlah saudara
            system("cls");
            printf("Masukkan jumlah saudara laki-laki : ");
            scanf("%d", &keluarga.sdrlk);
            printf("Masukkan jumlah saudara perempuan : ");
            scanf("%d", &keluarga.sdrpr);
            break;
          }
          else if(saudara == 0){ //mengosongkan semua variabel saudara
            keluarga.sdrlk = 0;
            keluarga.sdrpr = 0;
            break;
          }
          else{
            printf("Masukkan angka dengan benar ! : "); //error handling saudara
            scanf("%d", &saudara);
          }
        }
        system("cls");
        printf("Apakah pewaris memiliki orang tua yang masih hidup?");
        printf("\nJika iya, masukkan angka 1");
        printf("\nJika tidak, masukkan angka 0");
        printf("\n\nMasukkan angka : ");
        scanf("%d", &orangtua); //cek kondisi apakah pewaris memiliki orang tua yang masih hidup
        while(orangtua != 0 || orangtua != 1){ //error handling untuk orang tua pewaris
          if(orangtua == 1){ //meminta data orang tua
            printf("\n1. Bapak");
            printf("\n2. Ibu");
            printf("\n3. Keduanya");
            printf("\n\nMasukkan angka : ");
            scanf("%d", &pilih);
            if(pilih == 1){ //bapak masih hidup
              keluarga.bapak = 1;
              keluarga.ibu = 0;
              break;
            }
            else if(pilih == 2){ //ibu masih hidup
              keluarga.bapak = 0;
              keluarga.ibu = 1;
              break;
            }
            else if(pilih == 3){ //kedua orang tua masih hidup
              keluarga.bapak = 1;
              keluarga.ibu = 1;
              break;
            }
            else{
              printf("\nMasukkan angka dengan benar !\n\n"); //error handling untuk orang tua pewaris
            }
          }
          else if(orangtua == 0){ //keduanya telah tiada
            keluarga.bapak = 0;
            keluarga.ibu = 0;
            break;
          }
          else{
            printf("Masukkan angka dengan benar ! : "); //error handling orang tua
            scanf("%d", &orangtua);
          }
        }
        
        perhitungan(harta, warisan, keluarga, keturunan, saudara, orangtua); //algoritma perhitungan harta warisan
        list(keluarga); //list anggota keluarga yang mendapatkan warisan
        display(warisan); //menampilkan hasil perhitungan harta waris
		
			//end of codes block
			//---------------------------------------------------------
}


//--------------------------------------------------------
//This function was made by Fikri Afif Musyaffa

void perhitungan(int harta, int warisan[10], struct keluarga keluarga, int keturunan, int saudara, int orangtua){
	
	int i; //variabel for loop
	int sisa = 0; //harta waris yang tersisa
	int temp = 0; //menghitung pembagian sementara antara laki-laki dan perempuan
	int sisa_bapak = 0; //perhitungan sisa untuk bapak pewaris jika masih hidup
	
	#pragma omp for
	for(i = 0; i < 10; i++){
		warisan[i] = 0; //mengosongkan array
	}
	
	if(keluarga.suami == 1){ //algoritma menentukan harta yang didapatkan suami
		if(keluarga.anaklk >= 1){
			if(keluarga.anakpr >= 1){
				if(keluarga.cuculk >= 1){
					if(keluarga.cucupr >= 1){
						if(keluarga.sdrlk || keluarga.sdrpr >= 1){
							warisan[0] = harta / 2; //harta yg didapatkan jika semua penyataan terpenuhi
						}else warisan[0] = harta / 4;
					}else warisan[0] = harta / 4;
				}else warisan[0] = harta / 4;
			}else warisan[0] = harta / 4;
		}else warisan[0] = harta / 4;
	}
	sisa = harta - warisan[0]; //sisa harta setelah suami mendapatkan warisan
	if(keluarga.istri >= 1){ //algoritma menentukan harta yang didapatkan istri
		if(keluarga.anaklk >= 1){
			if(keluarga.anakpr >= 1){
				if(keluarga.cuculk >= 1){
					if(keluarga.cucupr >= 1){
						if(keluarga.sdrlk || keluarga.sdrpr >= 1){
							warisan[1] = (harta / 4) / keluarga.istri; //masing-masing istri mendapatkan satu per delapan
						}else warisan[1] = (harta / 8) / keluarga.istri;
					}else warisan[1] = (harta / 8) / keluarga.istri;
				}else warisan[1] = (harta / 8) / keluarga.istri;
			}else warisan[1] = (harta / 8) / keluarga.istri;
		}else warisan[1] = (harta / 8) / keluarga.istri;
	}
	sisa = harta - (warisan[1] * keluarga.istri); //sisa harta setelah istri mendapatkan warisan
	
	int tugas = 0;
	
	#pragma omp parallel private (tugas) //parallel programming menggunakan private threads
	{
		#pragma omp master //parallel master
		{
			for(i = 0; i < 3; i++){
				tugas = i; //supaya task yang dikerjakan berurutan
				#pragma omp task //parallel task
				{
					if(tugas == 1){ //loop pertama dieksekusi
						if(keluarga.ibu == 1){
							if(keluarga.anaklk >= 1 || keluarga.anakpr >= 1 || keluarga.cuculk >= 1 || keluarga.cucupr >= 1){ //pengecekan harta yang diperoleh
								#pragma omp critical //menghindari race condition
								{
									warisan[7] = harta / 6; //harta yang didapatkan ibu
									sisa = sisa - warisan[7]; //sisa setelah ibu mendapatkan harta		
								}
							}
							else{
								if(keluarga.ibu == 1){
									if((keluarga.suami == 1) || (keluarga.istri >= 1) || keluarga.bapak == 1){ //pengecekan syarat harta yang diperoleh
									#pragma omp critical //menghindari race condition
										{
											warisan[7] = harta / 3; //harta yang didapatkan ibu
											sisa = sisa - warisan[7]; //sisa setelah ibu mendapatkan harta	
										}
									}
								}	
							}
						}	
					}
					if(tugas == 2){ //loop kedua dieksekusi
						if(keluarga.bapak == 1 ){
							if(keluarga.anaklk >= 1 || keluarga.anakpr >= 1 ||keluarga.cuculk >= 1 || keluarga.cucupr >= 1){ //pengecekan syarat harta yang diperoleh	
								#pragma omp critical //menghindari race condition
								{			
									warisan[6] = harta / 6; //harta yang didapat bapak
									sisa = sisa - warisan[6]; //sisa setelah bapak mendapatkan harta
								}
							}
							else{
								#pragma omp critical //menghindari race condition
								{
									warisan[6] = harta / 6; //harta yang didapat bapak
									sisa = sisa - warisan[6]; //sisa setelah bapak mendapatkan harta
									sisa_bapak = 1;
								}
							}
						}
					}
				}
			}
		}
		#pragma omp taskwait //menghindari race condition
	}
	
	if(keluarga.anaklk >= 1){ //anak laki-laki 
		if(keluarga.anakpr >= 1){
			temp = (2 * keluarga.anaklk) + keluarga.anakpr; //perbandingan anak laki 2x lebih banyak dibanding perempuan
			warisan[2] = (2 * sisa) / temp; //harta yang didapatkan masing-masing anak laki-laki
			warisan[3] = sisa / temp; //harta yang didapatkan masing-masing anak perempuan
			sisa = sisa - (keluarga.anaklk * warisan[2] + keluarga.anakpr * warisan[3]);
		}
		else{ //jika anak perempuan tidak ada
			temp = (2 * keluarga.anaklk) + keluarga.anakpr; //perbandingan anak laki 2x lebih banyak dibanding perempuan
			warisan[2] = (2 * sisa) / temp;
			sisa = sisa - (keluarga.anaklk * warisan[2] + keluarga.anakpr * warisan[3]); //sisa harta			
		}
	}
	else if(keluarga.anaklk == 0){ //jika anak laki-laki tidak ada
		if(keluarga.anakpr == 1){ //anak perempuan sendiri
			warisan[3] = harta / 2; //harta anak perempuan
			sisa = sisa - warisan[3];
			if(keluarga.cuculk >= 1){
				warisan[8] = sisa / keluarga.cuculk; //harta masing-masing cucu laki-laki
				sisa = sisa - (warisan[8] * keluarga.cuculk);
			}
		}
		else if(keluarga.anakpr >= 2){ //anak perempuan lebih dari sama dengan 2
			warisan[3] = ((2 * harta) / 3) / keluarga.anakpr; //harta masing-masing anak perempuan
			sisa = sisa - (warisan[3] * keluarga.anakpr);
			if(keluarga.cuculk >= 1){
				warisan[8] = sisa / keluarga.cuculk; //harta masing-masing cucu laki-laki
				sisa = sisa - (warisan[8] * keluarga.cuculk);
			}
		}
		else if(keluarga.anakpr == 0){ //jika anak perempuan tidak ada
			if(keluarga.cuculk >= 1){ //jika cucu laki ada, maka pembagian sebagai berikut
				if(keluarga.cucupr >= 1){
					temp = (2 * keluarga.cuculk) + keluarga.cucupr;
					warisan[8] = (2 * sisa) / temp; //cucu laki-laki 2x cucu perempuan
					warisan[9] = sisa / temp;
					sisa = sisa - (keluarga.cuculk * warisan[8] + keluarga.cucupr * warisan[9]);
				}
				else if(keluarga.cuculk == 0){ //jika cucu laki tidak ada, maka pembagian sebagai berikut
					if(keluarga.anakpr >= 1){ //jika anak perempuan ada
						warisan[9] = (harta / 6) / keluarga.cucupr; //harta masing-masing cucu perempuan
						sisa = sisa - (warisan[9] * keluarga.cucupr);
					}
					else if(keluarga.cucupr == 1){ //cucu perempuan hanya 1
						warisan[9] = harta / 2; //harta cucu perempuan
						sisa = sisa - warisan[9];
					}
					else if(keluarga.cucupr >= 2){ //cucu perempuan lebih besar sama dengan 2
						warisan[9] = ((2 * harta) / 3) / keluarga.cucupr; //harta masing-masing cucu perempuan
						sisa = sisa - (warisan[9] * keluarga.cucupr);
					}
				}
			}
			else if(keluarga.cuculk == 0){ //jika cucu laki tidak ada
				if(keluarga.sdrlk >= 1){ //jika saudara laki ada
					if(keluarga.sdrpr >= 1){ //jika saudara perempuan ada
						temp = (2 * keluarga.sdrlk) + keluarga.sdrpr;
						warisan[4] = (2 * sisa) / temp; //laki-laki mendapatkan 2x perempuan
						warisan[5] = sisa / temp;
						sisa = sisa - (keluarga.sdrlk * warisan[4] + keluarga.sdrpr * warisan[5]);
					}
				}
				else if(keluarga.sdrlk == 0){ //jika saudara laki tidak ada
					if(keluarga.sdrpr == 1){
						warisan[5] = harta / 2; //harta saudara perempuan
						sisa = sisa - warisan[5];						
					}
					else if(keluarga.sdrpr >= 2){ //jika saudara perempuan lebih dari sama dengan 2
						warisan[5] = ((2 * harta) / 3) / keluarga.sdrpr; //harta masing-masing saudara perempuan
						sisa = sisa - (warisan[5] * keluarga.sdrpr);
					}
				}
			}
		}
	}
	
	if(sisa_bapak == 1 && sisa > 0){ //jika masih ada sisa, maka sisa tersebut milik bapak pewaris
		warisan[6] += sisa; //harta akhir bapak
	}
	#pragma omp for //pembagian for loop untuk setiap threads
	for(i = 0; i < 10; i++){ //error handling jika ada yang mendapatkan harta negatif
		if(warisan[i] < 0){
			warisan[i] = 0;
		}
	}
	printf("\nSedang menghitung pembagian harta warisan . . . \n\n");
	sleep(3); //jeda 3 detik
	system("cls");
	printf("SISA Harta Pewaris : %d\n\n", sisa); //sisa akhir perhitungan
}
//end of codes block
//--------------------------------------------------------


//--------------------------------------------------------
//This function was made by Fikri Afif Musyaffa

void list(struct keluarga keluarga){ //menampilkan jumlah anggota keluarga dari masing-masing kategori
	printf("Key list anggota keluarga\n\n");
	printf("1. Suami : %d\n", keluarga.suami);
	printf("2. Istri : %d\n", keluarga.istri);
	printf("3. Anak Laki-laki : %d\n", keluarga.anaklk);
	printf("4. Anak Perempuan : %d\n", keluarga.anakpr);
	printf("5. Saudara Laki-laki : %d\n", keluarga.sdrlk);
	printf("6. Saudara Perempuan : %d\n", keluarga.sdrpr);
	printf("7. Bapak : %d\n", keluarga.bapak);
	printf("8. Ibu : %d\n", keluarga.ibu);
	printf("9. Cucu Laki-laki : %d\n", keluarga.cuculk);
	printf("10. Cucu Perempuan : %d\n", keluarga.cucupr);
}

//end of codes block
//--------------------------------------------------------


//--------------------------------------------------------
//This function was made by Fikri Afif Musyaffa

void display(int warisan[10]){ //menampilkan harta yang telah dibagi sesuai aturan waris
	
	int i;
	
	#pragma omp for //pembagian for loop ke masing-masing threads
	for(i = 0; i < 10; i++){
		printf("\nKategori anggota keluarga ke-%d mendapatkan masing-masing : Rp. %d", i+1, warisan[i]);
	}
}

//end of codes block
//--------------------------------------------------------
