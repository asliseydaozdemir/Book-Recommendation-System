#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
int **oldUserData; //eski kullan�c�lar�n verilerini tutan dizi
int **newUserData; //yeni kullan�c�lar�n verilerini tutan dizi
double *maxArr;
int count = 0, inputValue; //kullan�c�dan bir kere al�n�p program boyunca de�i�meyecek de�erler.
int trainData, data;//dosyadan al�nan eski ve yeni okuyucu say�s�
//okunmu� olan kitaplar i�in aritmetik ortalamay� bulan fonksiyon
double arithmetic_meanv2(int * data, int size) //boyut ve diziyi parametre olarak al�r
{
    double total = 0; //toplam de�erin saklanaca�� de�i�ken
    int count = size; //ortalamada boyut
    int i;
    for(i = 0; i < size; i++){
    	if(!data[i]){ //e�er okunmad�ysa 
    		count--;	
		}
    	else
    		total += data[i]; //okunduysa 
	}
    return total / count; //aritmetik ortalamay� d�nd�r�r.
}
//predict i�in ikisinin de okudu�u ortak kitaplar� bulan art. ort.
double arithmetic_mean(int * a, int *b, int size)
{
    double total = 0;  //toplam de�erin saklanaca�� de�i�ken
    double reg = size;  //ortalamada boyut
    int i;
    for(i = 0; i < size; i++){
    	if(a[i] * b[i] == 0){ //e�er okunmad�ysa 
    		reg--;	
		}
    	else
    		total += a[i]; //okunduysa 
	}
    return total / reg; //aritmetik ortalamay� d�nd�r�r.
}
//iki okuyucu aras�ndaki similarity oran�n� bulan fonks.
double sim(int a[count], int b[count]){
	double mean1 = arithmetic_mean(a,b, count); //ortalama hesab�
	double mean2 = arithmetic_mean(b,a, count); //ortalama hesab�
	int i;
	double cov = 0.0, normalize_1 = 0.0, normalize_2 = 0.0, similar = 0.0; 
	for(i = 0; i < count; i++){
		if(a[i]*b[i] == 0); //ikisinden biri rate edilmediyse
		else{
		cov += (a[i] - mean1) * (b[i] - mean2);//covarience bulur
		normalize_1 += pow(a[i] - mean1, 2);//payda i�in normalizasyon i�lemleri
		normalize_2 += pow(b[i] - mean2, 2);	//payda i�in normalizasyon i�lemleri
		}
	}
	similar = cov / (sqrt(normalize_1) * sqrt(normalize_2));//similaritynin bulunmas�
	return similar;
}
//kitap tahmini yapan fonksiyon
void predict(int index, int closest[inputValue], char books[count][30]){ //kitaplar�, kullan�c�n�n indeksini, en yak�n de�erleri parametre olarak alan fonks.
	double meanofNewUser = arithmetic_meanv2(newUserData[index], count); //yeni kullan�c� i�in aritmetik ortalamay� bulmaya yarar
	double sum = 0, divide, max; //predict i�in kullan�lan de�i�kenler
	int indexv2 = 0, i, j;
	for(i = 0; i < count; i++){
		if(newUserData[index][i] == 0){
			indexv2++;  //ka� tane okumad��� kitap oldu�unu bulur.
		}		
	}
	int *booksNotRead = booksNotRead = (int *)malloc(indexv2 * sizeof(int));
	indexv2 = 0;
		for(i = 0; i < count; i++){
		if(newUserData[index][i] == 0){
			booksNotRead[indexv2] = i; //okunmayan kitaplar�n bulunmas�n� sa�lar
			indexv2++;
		}		
	}
	double pred[indexv2][indexv2]; //kitab� ve o kitap i�in predict de�erini tutacak bir matris tan�mlan�r.
	for(j = 0; j < indexv2; j++){
		for(i = 0; i < inputValue; i++){
			//prediction�n yap�labilmesi i�in kullan�l�r.-->verilen tahmin fonksiyonuna g�re
			sum += sim(newUserData[index], oldUserData[closest[i]]) * (oldUserData[closest[i]][booksNotRead[j]] - arithmetic_meanv2(oldUserData[closest[i]], count));
			divide += sim(newUserData[index], oldUserData[closest[i]]); 
		}
		pred[0][j] = (sum / divide) + meanofNewUser; //predict de�eri
		pred[1][j] = booksNotRead[j]; //tavsiye edilecek kitap.
		sum = 0;
		divide = 0;
	}
	printf("\n");
	max = pred[0][0]; //en y�ksek oranl� kitap i�in ilklendirme
	index = 0;
		for(i = 0; i < indexv2; i++){
			printf("\n%s has --->", books[(int)pred[1][i]]);  //kitab� yazd�r�r
			printf(" %f rating", pred[0][i]);  //kitab�n oran�n� yazd�rmaya yarar
			if(max < pred[0][i]){
				max = pred[0][i]; //max de�erinin bulunmas�
				index = i;
			}
		}
	printf("\n\nbooks that reccomended for user: %s", books[(int)pred[1][index]]); //kitab�n �nerilmesi
	printf("\n-----------------------------------------------");
}
//maksimum de�er bulan fonksiyon
int findMax(int size){
	int c, location = 0;
	int number = 0;
	for (c = 1; c < size; c++)
    if (maxArr[c]> maxArr[location]){ //b�y�kl�k kar��la�t�rmas� -> O(N) complexity
    	location = c;
    	number = location; 
	}
     maxArr[location] = -99; //bir sonraki max i�leminde ayn� de�ere bak�lmamas�n� sa�lar.
     return number; //indisi d�nd�rmeye yarar
}
//similarity i�in gerekli ayarlamalar� yapan fonksiyon
int findSimilarity(int data, char similarityUser[10], char books[count][30],char oldUsers[data][count], char newUsers[trainData][count]){
	int i = 0, index, loop = 0;
	int closest[inputValue]; //en yak�n de�erleri tutacak  dizi
	double *similarity, *tempSimilarity, distance;
	similarity = (double*)malloc(data * sizeof(double)); //benzerlik oranlar�n� tutacak dizi
	maxArr = (double*)malloc(data * sizeof(double));
	while(strcmp(similarityUser, newUsers[i])) {
		i++; //yeni kullan�c�n�n bulunmas� 
	}
	index = i;
	for(i = 0; i < data; i++){
		similarity[i] = sim(oldUserData[i], newUserData[index]); // benzerli�in bulunmas�
	}
	for(loop = 0; loop < data; loop++) {
      maxArr[loop] = similarity[loop]; //kopyalama i�lemi 
   }
   for(i = 0; i < inputValue; i++){
   		closest[i] = findMax(data); // max bulmaya yarar benzerlik buldu�umuz i�in karma��kl�k sayi * o(n) kadar olup sonu�ta O(n) getirecektir
   }
   printf("\n\nmost similar users for %s with similaritites: ", similarityUser);
   for(i = 0; i < inputValue; i++){
   	 	printf("\n %s, %f", oldUsers + closest[i], similarity[closest[i]]); //benzerliklerin ve kitaplar�n ekrana verilmesi.
   }
   predict(index, closest, books);
}

void readFile(char similarityUser[10], int n){
	FILE *fp = fopen("RecomendationDataSet.csv", "r");  //dosyan�n okunmak �zere a��lmas�
	int i = 0, control = 0, olddata = 0; 
	char line[256], booksLine[256], name[8]; //dosyadan sat�r sat�r okumaya yarayacak de�i�ken tan�mlar�
	int row = 0, columns, number;
    if (fp == NULL) {
        printf("Cannot open file \n");//dosya a��lmad�
        exit(0);
	} 
	//eski kullan�c� say�s�n�n al�nmas�
	fgets(line, sizeof(line), fp);
	data = atoi(line);
	//yeni kullan�c� say�s�n�n al�nmas�
	fgets(line, sizeof(line), fp);
	trainData = atoi(line);
	//kitap sat�rlar�n�n al�nmas�
	fgets(line, sizeof(line), fp);
	strcpy(booksLine, line);
	char* token = strtok(line, ";");  
	token = strtok(NULL, ";");  
    while (token != NULL) { 
        count++; // kitap say�s�n�n bulunmas�
        token = strtok(NULL, ";");  // ilerleme
    } 
    char books[count][30]; //kitaplar�n al�nmas� i�in de�i�ken	
    i = 0;
    token = strtok(booksLine, ";"); 
	token = strtok(NULL, ";"); 
    while (token != NULL) { 
    	strcpy(books[i], token); //kitaplar�n al�nmas� 
		i++; 
        token = strtok(NULL, ";"); 
    } 	
    for(i = 0; i < strlen(books[count -1]); i++);
    books[count-1][i-1] = NULL; //son kitaptaki \n karakterinin at�lmas�
    oldUserData = (int **)malloc(data * sizeof(int*)); //eski kullan�c� say�s�na g�re memory allocation'�
	for(i = 0; i < data; i++) 
		oldUserData[i] = (int *)malloc(count * sizeof(int));//eski kullan�c�lar i�in memory allocation'�
		
	newUserData = (int **)malloc(trainData * sizeof(int*)); //kullan�c� say�s�na g�re memory allocation'�
	for(i = 0; i < trainData; i++) 
		newUserData[i] = (int *)malloc(count * sizeof(int));//yeni kullan�c�lar�n verilerini saklayacak memory allocation'�
	char oldUsers[data][count]; //eski user isimlerin al�nmas� i�in dizi
	char newUsers[trainData][count]; //yeni user isimlerin al�nmas� i�in dizi
	while (fgets(line, sizeof(line), fp)) {
		i = 0;	
		token = strtok(line, ";"); 
		strcpy(name, token); //ilk s�tundaki ismin al�nmas�
		token = strtok(NULL, ";"); 
		while (token != NULL) { 
			if(row < data){ //old userlar�n al�nmas�
			number = atoi(token); //al�nan de�er string oldu�u i�in cast i�leminin yap�lmas�
			strcpy(oldUsers[row], name); //eski userlar�n ad�n�n saklanmas�
    		oldUserData[row][i] = number; //eski userlar�n verilerinin saklanmas�
			i++; 
        	token = strtok(NULL, ";"); 
        	}
        	else{
        		number = atoi(token); //al�nan de�er string oldu�u i�in cast i�leminin yap�lmas�
        		strcpy(newUsers[row - data], name); //ismin verilmesi
        		newUserData[row - data][i] = number; //verilerin saklanmas�
				i++; 
        		token = strtok(NULL, ";"); 	
			}
    	} 
		row++;
	}	
	if(n == 1){
		findSimilarity(data, similarityUser, books, oldUsers, newUsers); //benzerlik bulmaya yarayan fonks.->spesifik bir kullan�c� i�in
	}
	else{
		for(i = 0; i < trainData; i++)
			findSimilarity(data, newUsers[i], books, oldUsers, newUsers); //benzerlik bulmaya yarayan fonks.->t�m kullan�c�lar i�in 
	}
}

int main(){
	char similarityUser[10]; //yeni kullan�c�n�n al�nmas�n� sa�layan tan�mlama
	int choice = 0, n; //kullan�c�n�n programa devam etmek isteyip istememesini alan de�i�ken
	printf("---------This is a book recommendation program-------\n");
	printf("-----------------------------------------------------\n");
	do{
		count = 0;
		printf("\nYour choices:\n1.Recommendation book for user whom you choose\n2.Recommendation book for all users");
		printf("\nEnter your choice 1 or 2: ");
		scanf("%d", &n);
		if(n == 1){
			printf("\nEnter your user for find similarity (like: NU1): ");
			scanf("%s", similarityUser);
			printf("Enter the K value: ");
			scanf("%d", &inputValue);
			readFile(similarityUser, n);	
		}
		else if(n == 2){
			printf("\nEnter the K value: ");
			scanf("%d", &inputValue);
			readFile(similarityUser, n);
		}
		else{
			printf("\nyou entered wrong value ");
			return 0;
		}
		printf("\n Do you want to exit if yes press 1 else press 0: ");
		scanf("%d", &choice);
	}while(choice == 0);
	return 0;
}
