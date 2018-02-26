#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct lab{
    int i;
    int yol;
    struct lab *sonraki;
}labirent;

labirent *labirentolustur(labirent *, int );
int cikisibul(labirent *, labirent **, int, int, int, int );

int main()
{
    srand(time(NULL));  //Program her calistiginda rand() icin yeni bir seed ugretilir.
    labirent *bas;  //Labirenti ve cozumunu saklamak icin linked listin ilk dugumleri olusturulur.
    labirent *cikisbas;
    labirent *aktif;
    bas = NULL;
    cikisbas = NULL;
    int labirentboyutu;
    do{
        printf("Labirentin boyutunu giriniz(Max: 48): ");
        scanf("%d", &labirentboyutu);
    }while(labirentboyutu>48 || labirentboyutu<1);
    int giris, cikis;
    int satir, sutun;
    bas = labirentolustur(bas, labirentboyutu);  //Labirent olusturulur.
    aktif = bas;
    while(aktif != NULL){  //Olusturulan labirent yazdirilir.
        printf("%d ", aktif->yol);
        if((aktif->i)%labirentboyutu == labirentboyutu-1){
            printf("\n");
        }
        aktif = aktif->sonraki;
    }
    do{                //Giris ve cikis kapilarinin konumlari kullanicidan alinir.
        aktif = bas;
        printf("\nGiris kapisinin hangi satir/sutunda oldugunu giriniz: ");
        printf("\nSatir: ");
        scanf("%d", &satir);
        printf("Sutun: ");
        scanf("%d", &sutun);
        giris = (satir-1)*labirentboyutu + sutun-1;
        while(aktif->i != giris){
            aktif = aktif->sonraki;
        }
    }while(aktif->yol != 1);
    aktif->yol = 2;
    do{
        aktif = bas;
        printf("\nCikis kapisinin hangi satir/sutunda oldugunu giriniz: ");
        printf("\nSatir: ");
        scanf("%d", &satir);
        printf("Sutun: ");
        scanf("%d", &sutun);
        cikis = (satir-1)*labirentboyutu + sutun-1;
        while(aktif->i != cikis){
            aktif = aktif->sonraki;
        }
    }while(aktif->yol != 1);
    printf("\n");            //Labirentte verilen giris ve cikis kapilarina gore yol aranir.
    if(!cikisibul(bas, &cikisbas, labirentboyutu, giris, cikis, -1 )){
       printf("\nYOL YOK.\n");
    }
    else{  //Yol bulundugu taktirde bulunan yol yazdirilir.
        printf("\nBulunan cikis yolu: \n");
        aktif = bas;
        while(aktif != NULL){
            if(aktif->yol == 2){
                aktif->yol = 1;
            }
            else{
                aktif->yol = 0;
            }
            printf("%d ", aktif->yol);
            aktif = aktif->sonraki;
            if(aktif == NULL || (aktif->i%(labirentboyutu) == 0 && aktif->i != 0)){
                printf("\n");
            }
        }
    }
    while(bas != NULL){  //Linked list olustururken dinamik olarak atadigimiz bellekler bosaltilir.
        aktif = bas;
        bas = bas->sonraki;
        free(aktif);
    }
    while(cikisbas != NULL){
        aktif = cikisbas;
        cikisbas = cikisbas->sonraki;
        free(aktif);
    }
    return 0;
}

labirent *labirentolustur(labirent *bas, int labirentboyutu){
    int i;    //Verilen boyuta gore rastgele duvar(0) ve yol(1)'lardan olusan bir labirent olusturulur.
    labirent *aktif;
    bas = malloc(sizeof(labirent));
    bas->sonraki = NULL;
    aktif = bas;
    for(i=0; i<round(pow(labirentboyutu, 2)); i++){
            if(i != 0){
                aktif->sonraki = malloc(sizeof(labirent));  //Labirentin saklanacagi linked listin dugumleri dinamik olarak olusturulur.
                aktif = aktif->sonraki;
            }
            aktif->i = i;         //Deger atamalari yapilir.
            aktif->yol = rand()%2;
    }
    aktif->sonraki = NULL;
    return bas;
}

int cikisibul(labirent *bas, labirent **cikisbas, int labirentboyutu, int konum, int cikis, int gelinenyon){
    labirent *aktif;       //Labirentte kullanicidan alinan giris ve cikislara gore cikis yolu bulunur.
    if(*cikisbas == NULL){   //Yol bulunurken yigin yapisi ve backtracking algoritmasi kullanilir.
        *cikisbas = malloc(sizeof(labirent));
        aktif = *cikisbas;
        aktif->sonraki = NULL;
    }                           //Bulunulan konum olusturdugumuz yigina atilir.
    else{
        aktif = malloc(sizeof(labirent));
        aktif->sonraki = *cikisbas;
        *cikisbas = aktif;
    }
    aktif->i = konum;
    aktif->yol = 1;
    if(konum == cikis){  //Eger cikisa ulasildiysa geriye 1 dondurulur.
        return 1;
    }
    int yon = (gelinenyon != 0) ? gelinenyon-1 : 3;  //Geldigimiz yone gore gidilecek yon belirlenir.
    while(yon != (gelinenyon+2)%4){
        if(gelinenyon == -1){
            yon = 0;
            gelinenyon = 2;
        }
        if(yon == 0 && konum > labirentboyutu-1){    //Bulundugumuz konumun bir ustune bakilir.
            aktif = bas;
            while(aktif->i != konum-labirentboyutu){
                aktif = aktif->sonraki;
            }
            if(aktif->yol == 1){  //Eger bir ustte yol var ise o yonden ilerlenir.
                aktif->yol = 0;
                if(cikisibul(bas, cikisbas, labirentboyutu, konum-labirentboyutu, cikis, 0)){
                    aktif->yol = 2;  //Rekursif olarak gittigimiz yerden yeni yollar aranir, eger sonuca
                    return 1;     //ulasilirsa 1 dondurulerek sonuca vardigimiz belirtilir.
                }        //Eger ki sonuca ulasilamazsa, diger yonlerde cikis aranmaya devam edilir.
            }
        }
        else if(yon == 1 && (konum)%labirentboyutu != 0){     //Bulundugumuz konumun bir soluna bakilir.
            aktif = bas;
            while(aktif->i != konum-1){
                aktif = aktif->sonraki;
            }
            if(aktif->yol == 1){
                aktif->yol = 0;
                if(cikisibul(bas, cikisbas, labirentboyutu, konum-1, cikis, 1)){
                    aktif->yol = 2;
                    return 1;
                }
            }
        }
        else if(yon == 2 && konum < labirentboyutu*(labirentboyutu-1)){     //Bulundugumuz konumun bir altina bakilir.
            aktif = bas;
            while(aktif->i != konum+labirentboyutu){
                aktif = aktif->sonraki;
            }
            if(aktif->yol == 1){
                aktif->yol = 0;
                if(cikisibul(bas, cikisbas, labirentboyutu, konum+labirentboyutu, cikis, 2)){
                    aktif->yol = 2;
                    return 1;
                }
            }
        }
        else if(yon == 3 && (konum)%labirentboyutu != labirentboyutu-1){     //Bulundugumuz konumun bir sagina bakilir.
            aktif = bas;
            while(aktif->i != konum+1){
                aktif = aktif->sonraki;
            }
            if(aktif->yol == 1){
                aktif->yol = 0;
                if(cikisibul(bas, cikisbas, labirentboyutu, konum+1, cikis, 3)){
                    aktif->yol = 2;
                    return 1;
                }
            }
        }
        yon = (yon+1)%4;
    }
    aktif = *cikisbas;         //Eger gidilebilecek hicbir yon sonuca ulasmazsa, bulundugumuz konum yigindan cikartilir.
    *cikisbas = aktif->sonraki;  //Ve geri donulur.
    free(aktif);
    return 0;
}
