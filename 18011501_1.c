#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
int **oldUserData; //eski kullanýcýlarýn verilerini tutan dizi
int **newUserData; //yeni kullanýcýlarýn verilerini tutan dizi
double *maxArr;
int count = 0, inputValue; //kullanýcýdan bir kere alýnýp program boyunca deðiþmeyecek deðerler.
int trainData, data;//dosyadan alýnan eski ve yeni okuyucu sayýsý
//okunmuþ olan kitaplar için aritmetik ortalamayý bulan fonksiyon
double arithmetic_meanv2(int * data, int size) //boyut ve diziyi parametre olarak alýr
{
    double total = 0; //toplam deðerin saklanacaðý deðiþken
    int count = size; //ortalamada boyut
    int i;
    for(i = 0; i < size; i++){
    	if(!data[i]){ //eðer okunmadýysa 
    		count--;	
		}
    	else
    		total += data[i]; //okunduysa 
	}
    return total / count; //aritmetik ortalamayý döndürür.
}
//predict için ikisinin de okuduðu ortak kitaplarý bulan art. ort.
double arithmetic_mean(int * a, int *b, int size)
{
    double total = 0;  //toplam deðerin saklanacaðý deðiþken
    double reg = size;  //ortalamada boyut
    int i;
    for(i = 0; i < size; i++){
    	if(a[i] * b[i] == 0){ //eðer okunmadýysa 
    		reg--;	
		}
    	else
    		total += a[i]; //okunduysa 
	}
    return total / reg; //aritmetik ortalamayý döndürür.
}
//iki okuyucu arasýndaki similarity oranýný bulan fonks.
double sim(int a[count], int b[count]){
	double mean1 = arithmetic_mean(a,b, count); //ortalama hesabý
	double mean2 = arithmetic_mean(b,a, count); //ortalama hesabý
	int i;
	double cov = 0.0, normalize_1 = 0.0, normalize_2 = 0.0, similar = 0.0; 
	for(i = 0; i < count; i++){
		if(a[i]*b[i] == 0); //ikisinden biri rate edilmediyse
		else{
		cov += (a[i] - mean1) * (b[i] - mean2);//covarience bulur
		normalize_1 += pow(a[i] - mean1, 2);//payda için normalizasyon iþlemleri
		normalize_2 += pow(b[i] - mean2, 2);	//payda için normalizasyon iþlemleri
		}
	}
	similar = cov / (sqrt(normalize_1) * sqrt(normalize_2));//similaritynin bulunmasý
	return similar;
}
//kitap tahmini yapan fonksiyon
void predict(int index, int closest[inputValue], char books[count][30]){ //kitaplarý, kullanýcýnýn indeksini, en yakýn deðerleri parametre olarak alan fonks.
	double meanofNewUser = arithmetic_meanv2(newUserData[index], count); //yeni kullanýcý için aritmetik ortalamayý bulmaya yarar
	double sum = 0, divide, max; //predict için kullanýlan deðiþkenler
	int indexv2 = 0, i, j;
	for(i = 0; i < count; i++){
		if(newUserData[index][i] == 0){
			indexv2++;  //kaç tane okumadýðý kitap olduðunu bulur.
		}		
	}
	int *booksNotRead = booksNotRead = (int *)malloc(indexv2 * sizeof(int));
	indexv2 = 0;
		for(i = 0; i < count; i++){
		if(newUserData[index][i] == 0){
			booksNotRead[indexv2] = i; //okunmayan kitaplarýn bulunmasýný saðlar
			indexv2++;
		}		
	}
	double pred[indexv2][indexv2]; //kitabý ve o kitap için predict deðerini tutacak bir matris tanýmlanýr.
	for(j = 0; j < indexv2; j++){
		for(i = 0; i < inputValue; i++){
			//predictionýn yapýlabilmesi için kullanýlýr.-->verilen tahmin fonksiyonuna göre
			sum += sim(newUserData[index], oldUserData[closest[i]]) * (oldUserData[closest[i]][booksNotRead[j]] - arithmetic_meanv2(oldUserData[closest[i]], count));
			divide += sim(newUserData[index], oldUserData[closest[i]]); 
		}
		pred[0][j] = (sum / divide) + meanofNewUser; //predict deðeri
		pred[1][j] = booksNotRead[j]; //tavsiye edilecek kitap.
		sum = 0;
		divide = 0;
	}
	printf("\n");
	max = pred[0][0]; //en yüksek oranlý kitap için ilklendirme
	index = 0;
		for(i = 0; i < indexv2; i++){
			printf("\n%s has --->", books[(int)pred[1][i]]);  //kitabý yazdýrýr
			printf(" %f rating", pred[0][i]);  //kitabýn oranýný yazdýrmaya yarar
			if(max < pred[0][i]){
				max = pred[0][i]; //max deðerinin bulunmasý
				index = i;
			}
		}
	printf("\n\nbooks that reccomended for user: %s", books[(int)pred[1][index]]); //kitabýn önerilmesi
	printf("\n-----------------------------------------------");
}
//maksimum deðer bulan fonksiyon
int findMax(int size){
	int c, location = 0;
	int number = 0;
	for (c = 1; c < size; c++)
    if (maxArr[c]> maxArr[location]){ //büyüklük karþýlaþtýrmasý -> O(N) complexity
    	location = c;
    	number = location; 
	}
     maxArr[location] = -99; //bir sonraki max iþleminde ayný deðere bakýlmamasýný saðlar.
     return number; //indisi döndürmeye yarar
}
//similarity için gerekli ayarlamalarý yapan fonksiyon
int findSimilarity(int data, char similarityUser[10], char books[count][30],char oldUsers[data][count], char newUsers[trainData][count]){
	int i = 0, index, loop = 0;
	int closest[inputValue]; //en yakýn deðerleri tutacak  dizi
	double *similarity, *tempSimilarity, distance;
	similarity = (double*)malloc(data * sizeof(double)); //benzerlik oranlarýný tutacak dizi
	maxArr = (double*)malloc(data * sizeof(double));
	while(strcmp(similarityUser, newUsers[i])) {
		i++; //yeni kullanýcýnýn bulunmasý 
	}
	index = i;
	for(i = 0; i < data; i++){
		similarity[i] = sim(oldUserData[i], newUserData[index]); // benzerliðin bulunmasý
	}
	for(loop = 0; loop < data; loop++) {
      maxArr[loop] = similarity[loop]; //kopyalama iþlemi 
   }
   for(i = 0; i < inputValue; i++){
   		closest[i] = findMax(data); // max bulmaya yarar benzerlik bulduðumuz için karmaþýklýk sayi * o(n) kadar olup sonuçta O(n) getirecektir
   }
   printf("\n\nmost similar users for %s with similaritites: ", similarityUser);
   for(i = 0; i < inputValue; i++){
   	 	printf("\n %s, %f", oldUsers + closest[i], similarity[closest[i]]); //benzerliklerin ve kitaplarýn ekrana verilmesi.
   }
   predict(index, closest, books);
}

void readFile(char similarityUser[10], int n){
	FILE *fp = fopen("RecomendationDataSet.csv", "r");  //dosyanýn okunmak üzere açýlmasý
	int i = 0, control = 0, olddata = 0; 
	char line[256], booksLine[256], name[8]; //dosyadan satýr satýr okumaya yarayacak deðiþken tanýmlarý
	int row = 0, columns, number;
    if (fp == NULL) {
        printf("Cannot open file \n");//dosya açýlmadý
        exit(0);
	} 
	//eski kullanýcý sayýsýnýn alýnmasý
	fgets(line, sizeof(line), fp);
	data = atoi(line);
	//yeni kullanýcý sayýsýnýn alýnmasý
	fgets(line, sizeof(line), fp);
	trainData = atoi(line);
	//kitap satýrlarýnýn alýnmasý
	fgets(line, sizeof(line), fp);
	strcpy(booksLine, line);
	char* token = strtok(line, ";");  
	token = strtok(NULL, ";");  
    while (token != NULL) { 
        count++; // kitap sayýsýnýn bulunmasý
        token = strtok(NULL, ";");  // ilerleme
    } 
    char books[count][30]; //kitaplarýn alýnmasý için deðiþken	
    i = 0;
    token = strtok(booksLine, ";"); 
	token = strtok(NULL, ";"); 
    while (token != NULL) { 
    	strcpy(books[i], token); //kitaplarýn alýnmasý 
		i++; 
        token = strtok(NULL, ";"); 
    } 	
    for(i = 0; i < strlen(books[count -1]); i++);
    books[count-1][i-1] = NULL; //son kitaptaki \n karakterinin atýlmasý
    oldUserData = (int **)malloc(data * sizeof(int*)); //eski kullanýcý sayýsýna göre memory allocation'ý
	for(i = 0; i < data; i++) 
		oldUserData[i] = (int *)malloc(count * sizeof(int));//eski kullanýcýlar için memory allocation'ý
		
	newUserData = (int **)malloc(trainData * sizeof(int*)); //kullanýcý sayýsýna göre memory allocation'ý
	for(i = 0; i < trainData; i++) 
		newUserData[i] = (int *)malloc(count * sizeof(int));//yeni kullanýcýlarýn verilerini saklayacak memory allocation'ý
	char oldUsers[data][count]; //eski user isimlerin alýnmasý için dizi
	char newUsers[trainData][count]; //yeni user isimlerin alýnmasý için dizi
	while (fgets(line, sizeof(line), fp)) {
		i = 0;	
		token = strtok(line, ";"); 
		strcpy(name, token); //ilk sütundaki ismin alýnmasý
		token = strtok(NULL, ";"); 
		while (token != NULL) { 
			if(row < data){ //old userlarýn alýnmasý
			number = atoi(token); //alýnan deðer string olduðu için cast iþleminin yapýlmasý
			strcpy(oldUsers[row], name); //eski userlarýn adýnýn saklanmasý
    		oldUserData[row][i] = number; //eski userlarýn verilerinin saklanmasý
			i++; 
        	token = strtok(NULL, ";"); 
        	}
        	else{
        		number = atoi(token); //alýnan deðer string olduðu için cast iþleminin yapýlmasý
        		strcpy(newUsers[row - data], name); //ismin verilmesi
        		newUserData[row - data][i] = number; //verilerin saklanmasý
				i++; 
        		token = strtok(NULL, ";"); 	
			}
    	} 
		row++;
	}	
	if(n == 1){
		findSimilarity(data, similarityUser, books, oldUsers, newUsers); //benzerlik bulmaya yarayan fonks.->spesifik bir kullanýcý için
	}
	else{
		for(i = 0; i < trainData; i++)
			findSimilarity(data, newUsers[i], books, oldUsers, newUsers); //benzerlik bulmaya yarayan fonks.->tüm kullanýcýlar için 
	}
}

int main(){
	char similarityUser[10]; //yeni kullanýcýnýn alýnmasýný saðlayan tanýmlama
	int choice = 0, n; //kullanýcýnýn programa devam etmek isteyip istememesini alan deðiþken
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
