#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include  <malloc.h>
#include <stdbool.h>


///DANS CE PROGRAMME E VA ETRE CONSIDERER NOTRE EPSILON


  ///les structures
     //transition Nodes
typedef struct contents{
int lineNumber;
int *startingNodes;
int *edingNodes;
char *etiquetes;}contents;
    //entrances and exit Nodes
typedef struct entring_exitNodes{
int entringNumber;
int exitNumber;
int *entrances;
int *exits;}entring_exitNodes;
    //regroupement des deux structures
typedef struct bothstructs{
    contents contentS;
    entring_exitNodes entring_exitNodeS;
}bothstructs;


////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///partie 1:

///////////////////////////////////////////////////////////////////////////////////

  ///nombre total des lignes
int numberLine(char* fileN){
    int i;
    char datatoberead[150];
    FILE *thefile=fopen(fileN,"r");
    if(thefile==NULL){
        printf("le fichier n'est pas ouvert\n");
    }
    i=0;
    while(fgets(datatoberead,150,thefile)!=NULL){
        i++;
    }
    //printf("%d\n", i);
    return i;
}

//////////////////////////////////////////////////////////////////////////////////

  ///fonction de stockage des transitions
 contents readAndstock(char* fileN){
    int transitionLines=numberLine(fileN)-2;
    int i,j,transitionNodes,a,ins,z,outs;
    transitionNodes=0;
    ins=0;
    outs=0;
    z=0;
    a=0;
    char datatoberead[150];

    FILE *thefile=fopen(fileN,"r");
    if(thefile== NULL){
        printf("le fichier n'est pas ouvert\n");
    }

    ///stockage de la 1ere structure
        //alignement du fichier pour un accés plus facile
    char *aligne=(char*)malloc((transitionLines*3)*sizeof(char));
        //aligné le fichier
    for(i=0;i<transitionLines;i++){
        fgets(datatoberead,150,thefile);
        for(j=0;datatoberead[j]!='\n' && datatoberead[j]!='\0';j++){
                 if(datatoberead[j]!='\t' && datatoberead[j]!=' '){
                    aligne[transitionNodes]=(datatoberead[j]);
                    j++;
                    transitionNodes++;
                 }
            }
    }

        //allocations des pointeurs de la 1ére structure
    contents *content = (contents*) malloc(sizeof(contents));
    content->edingNodes=(int*)malloc((transitionNodes/3)*sizeof(int));
    content->startingNodes=(int*)malloc((transitionNodes/3)*sizeof(int));
    content->etiquetes=(char*)malloc((transitionNodes/3)*sizeof(char));
    content->lineNumber=transitionLines;

        ///stockage
            //grace à l'allignement :
            //les debut ont toujours un indice multiple de 3 (et 0): 0 3 6 9 12....
            //les destinations ont : 1 4 7 10 13...
            //les etiquette ont: 2 5 8 11 14....
    for(i=0;i<transitionNodes;i+=3){
        content->startingNodes[a]=(aligne[i])-'0';
        content->edingNodes[a]=(aligne[i+1])-'0';
        content->etiquetes[a]=aligne[i+2];
        a++;
    }
return *content;
}

////////////////////////////////////////////////////////////////////////////////////////////


///stocker les entrees et les sorties
entring_exitNodes readAndstock2(char* fileN){

    int numberline=numberLine(fileN),j,z,ins,outs,i;
    char datatoberead[150];
    char *aligne=(char*)malloc(((numberline-2)*3)*sizeof(char));
    FILE *thefile=fopen(fileN,"r");
    z=0;
    ins=0;
    outs=0;

    ///stockage des deux derniers lignes
    for(i=0;i<numberline;i++){
            fgets(datatoberead,150,thefile);

        //savoir le nombre des entres
        if(i==numberline-2){
            for(j=0;datatoberead[j]!='\0' && datatoberead[j]!='\n';j++){
                    if(datatoberead[j]!=' ' && datatoberead[j]!='\t'){
                            aligne[z]=datatoberead[j];
                            z++;
                            ins++;
            }
          }
        }

        //savoir le nombre des sorties
        if(i==numberline-1){

            for(j=0;datatoberead[j]!='\0' && datatoberead[j]!='\n';j++){
                    if(datatoberead[j]!=' ' && datatoberead[j]!='\t'){
                            aligne[z]=datatoberead[j];
                            z++;
                            outs++;
            }
          }
        }
    }

    z=0;
        //allocations de la 2eme structure
    entring_exitNodes *eeNodes=(entring_exitNodes*)malloc(sizeof(entring_exitNodes));
    eeNodes->entringNumber=ins;
    eeNodes->exitNumber=outs;
    eeNodes->entrances=(int*)malloc(ins*sizeof(int));
    eeNodes->exits=(int*)malloc(outs*sizeof(int));

    ///stockage
    i=0;
    while(i<ins){
      eeNodes->entrances[i]=aligne[i]-'0';
      i++;
    }
    while(i<(ins+outs)){
        eeNodes->exits[z]=aligne[i]-'0';
        i++;
        z++;
    }
    return *eeNodes;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

///affichage des transitions
void affichage(char* fileN){
    int i,numberline;
    contents content;
    numberline=numberLine(fileN)-2;
    content=readAndstock(fileN);

    for(i=0;i<numberline;i++){
        printf("start:%d end:%d tag:%c\n", content.startingNodes[i],content.edingNodes[i],content.etiquetes[i]);
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////

///affichage des entrees et des sorties
void affichage2(char* fileN){

    int i,ins,outs;
    entring_exitNodes eeNodes;
    eeNodes=readAndstock2(fileN);
    ins=eeNodes.entringNumber;
    outs=eeNodes.exitNumber;



        //entrees
    printf("entree:");
    for(i=0;i<ins;i++){
        printf("%d\t", eeNodes.entrances[i]);
    }

    printf("\n");

        //sorties
    printf("sorties:");
    for(i=0;i<outs;i++){
        printf("%d\t", eeNodes.exits[i]);
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////

///creation du fichier dot
void creationDot(bothstructs a){
    int j,i;
    ///creation du fichier .dot
     FILE *diagramfile=fopen("diagram.dot","w");
     if(diagramfile==NULL){
        printf("fichier non cree\n");
        }
    fprintf(diagramfile,"%s\n %s\n","digraph{","bgcolor=lightblue");
    for(i=0;i<a.contentS.lineNumber;i++){
        fprintf(diagramfile,"%d->%d\t [label=%c] \n", a.contentS.startingNodes[i],a.contentS.edingNodes[i],a.contentS.etiquetes[i]);
    }


        for(j=0;j<a.entring_exitNodeS.entringNumber;j++){
                fprintf(diagramfile,"entrance%d->%d\t [color=red]\n",j,a.entring_exitNodeS.entrances[j]);
            }
        for(j=0;j<a.entring_exitNodeS.exitNumber;j++){
                fprintf(diagramfile,"%d->exit%d\t [color=red]\n",a.entring_exitNodeS.exits[j],j);
            }
    fprintf(diagramfile,"}");
    fclose(diagramfile);
    system("diagram.dot");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///partie2

//mettre les deux struscture dans une.
bothstructs readAndstock3(char* fileN){
    bothstructs all;
    all.contentS=readAndstock(fileN);
    all.entring_exitNodeS=readAndstock2(fileN);
    return all;
}


///////////////////////////////////////////////////////////////////////////////////////////

//creation de la fonction qui permet de decider si un mot appartient au langage reconnue
//par l'automate ou non
int mot_automate(bothstructs a,char m[]){

    int i,j,z,numberline,flag,etat,etatInitial,etatSuivant,exit,primFlag;
    char motNonLue;
    numberline=a.contentS.lineNumber;
    i=0;
    primFlag=0;
    exit=0;
    flag=0;
    z=0;
    motNonLue=m[i];
//tester si le mot peut integrer l'automate
        for(z=0;z<a.entring_exitNodeS.entringNumber;z++){
              for(j=0;j<numberline;j++){
                if(a.contentS.startingNodes[j]==a.entring_exitNodeS.entrances[z]){
                    if(a.contentS.etiquetes[j]==motNonLue){
                        i++;
                        motNonLue=m[i];
                        etatInitial=j;
                        primFlag=1;
                    }
                }
                if(primFlag==1){
	                j=numberline;
	              	z=a.entring_exitNodeS.entringNumber;
	              }
              }
            }
     if(i==0){
     	printf("\nle mot n'a pas pus integrer l'automate donc:\t");
        return -1;
     }

//tester les transitions
     else{
     z=1;
     while(motNonLue!='\0'){

       for(etatSuivant=0;etatSuivant<numberline;etatSuivant++){
            if(a.contentS.startingNodes[etatSuivant]==a.contentS.edingNodes[etatInitial]){
                if(a.contentS.etiquetes[etatSuivant]==motNonLue){
                    i++;
                    motNonLue=m[i];
                    etatInitial=etatSuivant;
                    break;
                }
                else if(a.contentS.etiquetes[etatSuivant]=='E'){ //E est lepsilon dans notre cas
                    etatInitial=etatSuivant;				     // condition sera outile pour la partie 3
                    z--;
                    break;
                    }
            }
       }

         if(z==i){
         	printf("\nbloquer au niveau du lettre Num %d donc:\t", i);
            break; //permet de quitter la boucle au cas de bloquage.
        }
        else{z++;}
     }


//tester si le mot atteint un terminale et était lis completement.
     for(i=0;i<a.entring_exitNodeS.exitNumber;i++){
        if(a.contentS.edingNodes[etatInitial]==a.entring_exitNodeS.exits[i]){
           exit=1;
        }
     }
    }
    if(motNonLue=='\0' && exit==1){
            flag=1;
        }
       return flag;
     }

////////////////////////////////////////////////////////////////////////////////////////


void wordSelect(bothstructs a,char filename[]){

    int i,wordNumber,len,flag;
    flag=0;
    char *ptr;
    FILE *wordFile=fopen(filename,"r");
    char dataToBeRead[150];
    wordNumber=0;
    while(fgets(dataToBeRead,150,wordFile)!=NULL){
        wordNumber++;
    }

    fclose(wordFile);

    FILE *wordFile2=fopen(filename,"r");

    for(i=0;i<wordNumber;i++){
            fgets(dataToBeRead,150,wordFile2);
            len=strlen(dataToBeRead);
            ptr=(char*)malloc(len*sizeof(char));
            ptr=dataToBeRead;
            ptr[len-1]='\0';
            flag=mot_automate(a,ptr);
            if(flag==1){
                printf("%s\n",ptr);
            }
            free(ptr);
        }

    fclose(wordFile2);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ///partie3

    //union de deux automates
    int unioN(bothstructs a, bothstructs b,char* m){
        int z, j, i,k,flag;
        i=0;
        k=0;
        for(z=0;z<a.entring_exitNodeS.entringNumber;z++){
              for(j=0;j<a.contentS.lineNumber;j++){
                if(a.contentS.startingNodes[j]==a.entring_exitNodeS.entrances[z]){
                    if(a.contentS.etiquetes[j]==m[0]){
                        i++;
                    }
                }
              }
        }
        for(z=0;z<b.entring_exitNodeS.entringNumber;z++){
              for(j=0;j<b.contentS.lineNumber;j++){
                if(b.contentS.startingNodes[j]==b.entring_exitNodeS.entrances[z]){
                    if(b.contentS.etiquetes[j]==m[0]){
                        k++;
                    }
                }
              }
        }
        if(i>0)
            flag=mot_automate(a,m);
        else if(k>0)
            flag=mot_automate(b,m);

        return flag;
    }

//////////////////////////////////////////////////////////////////////////////////////////////////

    //etoile d'un automate
bothstructs etoile(bothstructs a){
    int i;
    int k;
    char epsilon='E';
    int linenb=a.contentS.lineNumber;
    //on doit ajoyter des transitions pour lier les sorties et les entres
    for(i=0;i<a.entring_exitNodeS.exitNumber;i++){
            for(k=0;k<a.entring_exitNodeS.entringNumber;k++){
                    // allocation  d'espace supplimentaire pour les nouveau transitions
        a.contentS.edingNodes=realloc(a.contentS.edingNodes,(linenb+1)*sizeof(int));
        a.contentS.startingNodes=realloc(a.contentS.startingNodes,(linenb+1)*sizeof(int));
        a.contentS.etiquetes=realloc(a.contentS.etiquetes,(linenb+1)*sizeof(char));
                    // on boucle pour lier chaque sortie a tous les entres
        a.contentS.startingNodes[a.contentS.lineNumber]=a.entring_exitNodeS.exits[i];
        a.contentS.edingNodes[a.contentS.lineNumber]=a.entring_exitNodeS.entrances[k];
        a.contentS.etiquetes[a.contentS.lineNumber]=epsilon;
        a.contentS.lineNumber++;
        linenb++;
        }
    }
    return a;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

    //produit de deux automates
bothstructs produit(bothstructs a, bothstructs b){
    int i;
    int k;
    char epsilon='E';
    int linenb=a.contentS.lineNumber;
    for(i=0;i<a.entring_exitNodeS.exitNumber;i++){
            for(k=0;k<b.entring_exitNodeS.entringNumber;k++){
                    // allocation  d'espace supplimentaire pour les nouveau transitions
        a.contentS.edingNodes=realloc(a.contentS.edingNodes,(linenb+1)*sizeof(int));
        a.contentS.startingNodes=realloc(a.contentS.startingNodes,(linenb+1)*sizeof(int));
        a.contentS.etiquetes=realloc(a.contentS.etiquetes,(linenb+1)*sizeof(char));
        a.contentS.etiquetes=realloc(a.contentS.etiquetes,(linenb+1)*sizeof(char));
                    // on boucle pour lier chaque sortie du premier automate a tous les entres
                    // du deuxieme automate
        a.contentS.startingNodes[a.contentS.lineNumber]=a.entring_exitNodeS.exits[i];
        a.contentS.edingNodes[a.contentS.lineNumber]=b.entring_exitNodeS.entrances[k];
        a.contentS.etiquetes[a.contentS.lineNumber]=epsilon;

        a.contentS.lineNumber++;
        linenb++;
        }
    }
    for(i=0;i<b.contentS.lineNumber;i++){
            //on ajoute les transition du deuxieme automate pour creer la concatination
        a.contentS.edingNodes=realloc(a.contentS.edingNodes,(linenb+1)*sizeof(int));
        a.contentS.startingNodes=realloc(a.contentS.startingNodes,(linenb+1)*sizeof(int));
        a.contentS.etiquetes=realloc(a.contentS.etiquetes,(linenb+1)*sizeof(char));

        a.contentS.startingNodes[a.contentS.lineNumber]=b.contentS.startingNodes[i];
        a.contentS.edingNodes[a.contentS.lineNumber]=b.contentS.edingNodes[i];
        a.contentS.etiquetes[a.contentS.lineNumber]=b.contentS.etiquetes[i];

        a.contentS.lineNumber++;
        linenb++;
    }
    return a;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///partie4:
	//suppression des epsilon:
bothstructs suppression_E(bothstructs a){
	int startingNode,nextNode,z,i,j,endingNode,lineN;
	bothstructs b;
	
	for(startingNode=0;startingNode<a.contentS.lineNumber;startingNode++){
		if(a.contentS.etiquetes[startingNode]=='E'){
			nextNode=a.contentS.edingNodes[startingNode];
			endingNode=startingNode;
			z=1;
			j=0;
			while(z>j){
				j++;
				for(i=0;i<a.contentS.lineNumber;i++){
					if(a.contentS.startingNodes[i]==nextNode && a.contentS.etiquetes[i]=='E'){
						nextNode=a.contentS.edingNodes[i];
						endingNode=i;
						z++;
						i=-1;
					}
				}
			}
			printf("\nstartingNode=%d", startingNode);
			printf("\nendingNode=%d", endingNode);
			for(i=0;i<a.entring_exitNodeS.exitNumber;i++){
				if(a.contentS.edingNodes[endingNode]==a.entring_exitNodeS.exits[i]){
					a.entring_exitNodeS.exitNumber++;
					a.entring_exitNodeS.exits[a.entring_exitNodeS.exitNumber-1]=a.contentS.startingNodes[startingNode];
				}
			}
				lineN=a.contentS.lineNumber;
				for(i=0;i<lineN;i++){
					if(a.contentS.startingNodes[i]==a.contentS.edingNodes[endingNode]){
					a.contentS.lineNumber++;
					printf("\n%d",a.contentS.lineNumber);
					a.contentS.startingNodes=realloc(a.contentS.startingNodes,(a.contentS.lineNumber)*sizeof(int));
					a.contentS.edingNodes=realloc(a.contentS.edingNodes,(a.contentS.lineNumber)*sizeof(int));
					a.contentS.etiquetes=realloc(a.contentS.etiquetes,(a.contentS.lineNumber)*sizeof(char));
					a.contentS.startingNodes[a.contentS.lineNumber-1]=a.contentS.startingNodes[startingNode];
					a.contentS.edingNodes[a.contentS.lineNumber-1]=a.contentS.edingNodes[i];
					a.contentS.etiquetes[a.contentS.lineNumber-1]=a.contentS.etiquetes[i];
					//printf("\n%d %d %c",a.contentS.startingNodes[a.contentS.lineNumber-1],a.contentS.edingNodes[a.contentS.lineNumber-1],a.contentS.etiquetes[a.contentS.lineNumber-1] );
				}
			}	
			
		}
	}
		b.contentS.startingNodes=(int*)malloc(a.contentS.lineNumber*sizeof(int));
		b.contentS.edingNodes=(int*)malloc(a.contentS.lineNumber*sizeof(int));
		b.contentS.etiquetes=(char*)malloc(a.contentS.lineNumber*sizeof(char));
		b.entring_exitNodeS.exits=(int*)malloc(a.entring_exitNodeS.exitNumber*sizeof(int));
		b.entring_exitNodeS.entrances=(int*)malloc(a.entring_exitNodeS.entringNumber*sizeof(int));
		b.entring_exitNodeS.exitNumber=a.entring_exitNodeS.exitNumber;
		b.entring_exitNodeS.entringNumber=a.entring_exitNodeS.entringNumber;
	j=0;
	for(i=0;i<a.contentS.lineNumber;i++){
		if(a.contentS.etiquetes[i]!='E'){
			b.contentS.startingNodes[j]=a.contentS.startingNodes[i];
			b.contentS.edingNodes[j]=a.contentS.edingNodes[i];
			b.contentS.etiquetes[j]=a.contentS.etiquetes[i];
			j++;
		}
	}
	b.contentS.lineNumber=j;
	j=0;
	for(i=0;i<a.entring_exitNodeS.entringNumber;i++){
			b.entring_exitNodeS.entrances[j]=a.entring_exitNodeS.entrances[i];
			j++;
	}
	b.entring_exitNodeS.entringNumber=a.entring_exitNodeS.entringNumber;
	j=0;
	for(i=0;i<a.entring_exitNodeS.exitNumber;i++){
			b.entring_exitNodeS.exits[j]=a.entring_exitNodeS.exits[i];
			j++;
	}
	return b;
}



///la fonction principale
int main()
{
   char *m=(char*)malloc(50*sizeof(char));
   char *filename=(char*)malloc(150*sizeof(char));
   char fileN[25];
   char fileN2[25];
   int flag,nbrePartie,nbreQst;
   bothstructs a;
   bothstructs b;
   bothstructs v;
   bothstructs automate_E;
   
   strcpy(fileN,"test 2.txt");
   strcpy(fileN2,"test.txt");

   automate_E=readAndstock3("avecEpsilon.txt");
   a=readAndstock3(fileN);
   b=readAndstock3(fileN2);

while (nbrePartie!=5){
   printf("\tchoisir un chiffre:\n");
   printf("1-partie 1\n2-partie 2\n3-partie 3\n4-partie 4\n5-quitter\n");
   scanf("%d", &nbrePartie);

   ///partie1:


   if(nbrePartie==1){
        printf("\tchoisir le numero du question:\n1-afficher le fichier .txt apres stockage\n2-afficher le fichier .dot\n");
        scanf("%d", &nbreQst);

       if(nbreQst==1){
       affichage(fileN);
       affichage2(fileN);
       printf("\n");
        }

       if(nbreQst==2){
       creationDot(a);
        }
   }
   ///partie2:

 else if(nbrePartie==2){
        printf("\tchoisir le numero du question:\n1-tester un mot\n2-tester un fichier .txt\n");
        scanf("%d", &nbreQst);

    //premiere focnction

    if(nbreQst==1){
           printf("entrer un mot\n");
           scanf("%s", m);
           flag=mot_automate(a,m);
           if(flag==1){
           printf("le mot appartient au langage de l'automate\n");
            }
           else{
            printf("mot n'appartient pas au langage de l'automate\n");
                }
            }

    //deuxieme focnction
    else if(nbreQst==2){
       printf("entrer le nom de fichie sous la forme * filename.txt *:(dans nos cas mots2.txt ou mots.txt) \n");
       scanf("%s", filename);
       printf("\n");
       printf("les mots acceptes par l'automate:\n");
       wordSelect(a,filename);
      }
 }

 ///partie3:

 else if(nbrePartie==3){
        printf("\tchoisir le numero du question:\n1-union\n2-etoile\n3-concatenation\n");
        scanf("%d", &nbreQst);

    if(nbreQst==1){
           printf("entrer un mot\n");
           //gets(m);
           scanf("%s", m);
           flag=unioN(a,b,m);
           if(flag==1){
           printf("le mot appartient au langage de l'automate\n");
            }
           else{
            printf("mot n'appartient pas au langage de l'automate\n");
                }
            }


    if(nbreQst==2){
    	   int choix2;
           v=etoile(a);
           a=readAndstock3(fileN);// on affecte a a l'automate original
           while(choix2!=3){
    	   printf("\tvoulez-vous :\n1-tester un mot\n2-voir le diagram de l'etoile \n3-revenir au menu principal\n");
    	   scanf("%d", &choix2);
    	   if(choix2==1){
	           printf("entrer un mot\n");
	           scanf("%s", m);
	           flag=mot_automate(v,m);
	           if(flag==1){
	           printf("le mot appartient au langage de l'automate\n");
	            }
	           else{
	            printf("mot n'appartient pas au langage de l'automate\n");
	            }
        	}
        	if(choix2==2)
        		creationDot(v);
   		}
    }

    if(nbreQst==3){
           v=produit(a,b);
           printf("entrer un mot\n");
           //gets(m);
           scanf("%s", m);
           flag=mot_automate(v,m);
           if(flag==1){
           printf("le mot appartient au langage de l'automate\n");
            }
           else{
            printf("mot n'appartient pas au langage de l'automate\n");
            }
        }
 	}
 	
 	///partie4:
	 	
 	else if(nbrePartie==4){
        printf("\tchoisir le numero du question:\n1-suppression des epsilon\n2-determinisation\n3-minimisation\n");
        scanf("%d", &nbreQst);
        if(nbreQst==1){
            //creationDot(automate_E);
            v=suppression_E(automate_E);
        	creationDot(v);
		}
    }   
}
    return 0;
}
