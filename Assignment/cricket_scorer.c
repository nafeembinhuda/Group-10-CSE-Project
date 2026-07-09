#include <stdio.h>
#include <string.h>

struct batsman //batsmen info
{
    char name[30];
    int runs;
    int ball;
    int fours;
    int sixes;
    float strikerate;
};

struct bowler //bowlers info
{
    char name[30];
    int conceded;
    int bowled;
    float economy;
    int wicket;
};

struct team //team info
{
    char teamname[20];
    struct batsman batsmen[11];
    struct bowler bowlers[11];
    int scored;
    int wickets;
    int extras;
};

int main()
{
    struct team team1,team2;

    printf("Enter team 1 name (to bat first): ");
    fgets(team1.teamname,sizeof(team1.teamname),stdin);
    team1.teamname[strcspn(team1.teamname,"\n")]='\0'; //assigned to bat first after the toss

    printf("Enter team 2 name (chasing team): ");
    fgets(team2.teamname,sizeof(team2.teamname),stdin);
    team2.teamname[strcspn(team2.teamname,"\n")]='\0';

    printf("\n");

    int i,j,k,n,w,s; //i, j, k for loops, n for over number, w, s for wicket scenario
    int o1=0;
    int o2=0;
    int b1=0;
    int b2=0;
    // these are for overs played by the team

    for(i=0;i<11;i++){
         printf("%s Player %d name: ",team1.teamname,i+1);
         fgets(team1.batsmen[i].name,sizeof(team1.batsmen[i].name),stdin);
         team1.batsmen[i].name[strcspn(team1.batsmen[i].name,"\n")]='\0';
         strcpy(team1.bowlers[i].name,team1.batsmen[i].name);
    }

    printf("\n");

    for(i=0;i<11;i++){
         printf("%s Player %d name: ",team2.teamname,i+1);
         fgets(team2.batsmen[i].name,sizeof(team2.batsmen[i].name),stdin);
         team2.batsmen[i].name[strcspn(team2.batsmen[i].name,"\n")]='\0';
         strcpy(team2.bowlers[i].name,team2.batsmen[i].name);
    }

printf("\n");

    printf("Enter number of overs: ");
    scanf("%d",&n);
    printf("\n");

    char bowled[10]; //to input the type of delivery

    team1.scored = 0;
    team1.wickets = 0;
    team1.extras = 0;
    team2.scored = 0;
    team2.wickets = 0;
    team2.extras = 0;

    for(i=0;i<11;i++)
{
    team1.batsmen[i].runs = 0;
    team1.batsmen[i].ball = 0;
    team1.batsmen[i].fours = 0;
    team1.batsmen[i].sixes = 0;

    team1.bowlers[i].conceded = 0;
    team1.bowlers[i].bowled = 0;
    team1.bowlers[i].wicket = 0;
    team1.bowlers[i].economy = 0;
}

    for(i=0;i<11;i++)
{
    team2.batsmen[i].runs = 0;
    team2.batsmen[i].ball = 0;
    team2.batsmen[i].fours = 0;
    team2.batsmen[i].sixes = 0;

    team2.bowlers[i].conceded = 0;
    team2.bowlers[i].bowled = 0;
    team2.bowlers[i].wicket = 0;
    team2.bowlers[i].economy = 0;
}

    printf("\n%s Innings: \n\n",team1.teamname);

    int striker,nonstriker; //the batsmen on the crease
    int x; //temporary variable for the striker and nonstriker
    int next; //next batsman to come after a wicket
    int bowling; //for current bowler

    printf("Select striker: ");
    scanf("%d",&striker);
    printf("Select Non-striker: ");
    scanf("%d",&nonstriker);
    striker--;
    nonstriker--;
    //as array permits values from 1 to 10

    for(i=0;i<n;i++){
            printf("Select bowler: ");
    scanf("%d",&bowling);
    bowling--;

        for(j=0;j<6;j++){
            printf("Over %d.%d: ",i,j+1);
            scanf("%s",bowled);

            if(strcmp(bowled,"wd")==0){
                team1.scored++;
                team1.extras++;
                team2.bowlers[bowling].conceded++;
                j--;
                //for wides
                //extras don't count as legal deliveries
            }
            else if(strcmp(bowled,"wd+1")==0){
                team1.scored=team1.scored+2;
                team1.extras=team1.extras+2;
                team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+2;
                j--;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
            }
            else if(strcmp(bowled,"wd+2")==0){
                team1.scored=team1.scored+3;
                team1.extras=team1.extras+3;
                team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+3;
                j--;
            }
            else if(strcmp(bowled,"wd+3")==0){
                team1.scored=team1.scored+4;
                team1.extras=team1.extras+4;
                team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+4;
                j--;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
            }
            else if(strcmp(bowled,"wd+4")==0){
                team1.scored=team1.scored+5;
                team1.extras=team1.extras+5;
                team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+5;
                j--;
            }
            else if(strcmp(bowled,"nb")==0){
                team1.scored=team1.scored+1;
                team1.extras=team1.extras+1;
                team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+1;
                team1.batsmen[striker].ball++;
                j--;
                //for no balls
            }
            else if(strcmp(bowled,"nb+1")==0){
                team1.scored=team1.scored+2;
                team1.extras=team1.extras+1;
                team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+2;
                team1.batsmen[striker].runs=team1.batsmen[striker].runs+1;
                team1.batsmen[striker].ball++;
                j--;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
            }
            else if(strcmp(bowled,"nb+2")==0){
                team1.scored=team1.scored+3;
                team1.extras=team1.extras+1;
                team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+3;
                team1.batsmen[striker].runs=team1.batsmen[striker].runs+2;
                team1.batsmen[striker].ball++;
                j--;
            }
            else if(strcmp(bowled,"nb+3")==0){
                team1.scored=team1.scored+4;
                team1.extras=team1.extras+1;
                team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+4;
                team1.batsmen[striker].runs=team1.batsmen[striker].runs+3;
                team1.batsmen[striker].ball++;
                j--;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
            }
            else if(strcmp(bowled,"nb+4")==0){
                team1.scored=team1.scored+5;
                team1.extras=team1.extras+1;
                team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+5;
                team1.batsmen[striker].runs=team1.batsmen[striker].runs+4;
                team1.batsmen[striker].fours++;
                team1.batsmen[striker].ball++;
                j--;
            }
            else if(strcmp(bowled,"nb+5")==0){
                team1.scored=team1.scored+6;
                team1.extras=team1.extras+1;
                team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+6;
                team1.batsmen[striker].runs=team1.batsmen[striker].runs+5;
                team1.batsmen[striker].ball++;
                j--;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
            }
            else if(strcmp(bowled,"nb+6")==0){
                team1.scored=team1.scored+7;
                team1.extras=team1.extras+1;
                team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+7;
                team1.batsmen[striker].runs=team1.batsmen[striker].runs+6;
                team1.batsmen[striker].sixes++;
                team1.batsmen[striker].ball++;
                j--;
            }
            else if(strcmp(bowled,"by")==0){
                team1.scored=team1.scored+1;
                team1.extras=team1.extras+1;
                team1.batsmen[striker].ball++;
                team2.bowlers[bowling].bowled++;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
                //for byes
            }
            else if(strcmp(bowled,"by2")==0){
                team1.scored=team1.scored+2;
                team1.extras=team1.extras+2;
                team1.batsmen[striker].ball++;
                team2.bowlers[bowling].bowled++;
            }
            else if(strcmp(bowled,"by3")==0){
                team1.scored=team1.scored+3;
                team1.extras=team1.extras+3;
                team1.batsmen[striker].ball++;
                team2.bowlers[bowling].bowled++;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
            }
            else if(strcmp(bowled,"by4")==0){
                team1.scored=team1.scored+4;
                team1.extras=team1.extras+4;
                team1.batsmen[striker].ball++;
                team2.bowlers[bowling].bowled++;
            }
             else if(strcmp(bowled,"by5")==0){
                team1.scored=team1.scored+5;
                team1.extras=team1.extras+5;
                team1.batsmen[striker].ball++;
                team2.bowlers[bowling].bowled++;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
            }
             else if(strcmp(bowled,"lb")==0){
                team1.scored=team1.scored+1;
                team1.extras=team1.extras+1;
                team1.batsmen[striker].ball++;
                team2.bowlers[bowling].bowled++;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
                //leg byes don't count for runs conceded by bowlers
            }
            else if(strcmp(bowled,"lb2")==0){
                team1.scored=team1.scored+2;
                team1.extras=team1.extras+2;
                team1.batsmen[striker].ball++;
                team2.bowlers[bowling].bowled++;
            }
            else if(strcmp(bowled,"lb3")==0){
                team1.scored=team1.scored+3;
                team1.extras=team1.extras+3;
                team1.batsmen[striker].ball++;
                team2.bowlers[bowling].bowled++;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
            }
            else if(strcmp(bowled,"lb4")==0){
                team1.scored=team1.scored+4;
                team1.extras=team1.extras+4;
                team1.batsmen[striker].ball++;
                team2.bowlers[bowling].bowled++;
            }
               else if(strcmp(bowled,"0")==0){
                team1.batsmen[striker].ball++;
                team2.bowlers[bowling].bowled++;

            }
            else if(strcmp(bowled,"1")==0){
                team1.scored=team1.scored+1;
                team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+1;
                team1.batsmen[striker].runs=team1.batsmen[striker].runs+1;
                team1.batsmen[striker].ball++;
                team2.bowlers[bowling].bowled++;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
            }
            else if(strcmp(bowled,"2")==0){
                team1.scored=team1.scored+2;
                team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+2;
                team1.batsmen[striker].runs=team1.batsmen[striker].runs+2;
                team1.batsmen[striker].ball++;
                team2.bowlers[bowling].bowled++;
            }
            else if(strcmp(bowled,"3")==0){
                team1.scored=team1.scored+3;
                team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+3;
                team1.batsmen[striker].runs=team1.batsmen[striker].runs+3;
                team1.batsmen[striker].ball++;
                team2.bowlers[bowling].bowled++;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
            }
            else if(strcmp(bowled,"4")==0){
                team1.scored=team1.scored+4;
                team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+4;
                team1.batsmen[striker].runs=team1.batsmen[striker].runs+4;
                team1.batsmen[striker].ball++;
                team2.bowlers[bowling].bowled++;
                team1.batsmen[striker].fours++;
            }
            else if(strcmp(bowled,"5")==0){
                team1.scored=team1.scored+5;
                team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+5;
                team1.batsmen[striker].runs=team1.batsmen[striker].runs+5;
                team1.batsmen[striker].ball++;
                team2.bowlers[bowling].bowled++;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
            }
            else if(strcmp(bowled,"6")==0){
                team1.scored=team1.scored+6;
                team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+6;
                team1.batsmen[striker].runs=team1.batsmen[striker].runs+6;
                team1.batsmen[striker].ball++;
                team2.bowlers[bowling].bowled++;
                team1.batsmen[striker].sixes++;
            }
            else if(strcmp(bowled,"w+1")==0){
                     team1.wickets++;
                     team1.scored=team1.scored+1;
                     team1.batsmen[striker].ball++;
                     team1.batsmen[striker].runs=team1.batsmen[striker].runs+1;
                     team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+1;
                     team2.bowlers[bowling].bowled++;
                     x=striker;
                     striker=nonstriker;
                     nonstriker=x;
                if(team1.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    break;
                }
                    printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    printf("Who was out (if striker then 1, if non striker then 2): ");
                    scanf("%d",&s);
                    if(s==1){
                    striker=next;
                    }
                    else if(s==2){
                    nonstriker=next;
                    }
                }
                else if(strcmp(bowled,"w+2")==0){
                     team1.wickets++;
                     team1.scored=team1.scored+2;
                     team1.batsmen[striker].ball++;
                     team1.batsmen[striker].runs=team1.batsmen[striker].runs+2;
                     team2.bowlers[bowling].conceded=team2.bowlers[bowling].conceded+2;
                     team2.bowlers[bowling].bowled++;
                if(team1.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    break;
                }
                    printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    printf("Who was out (if striker then 1, if non striker then 2): ");
                    scanf("%d",&s);
                    if(s==1){
                    striker=next;
                    }
                    else if(s==2){
                    nonstriker=next;
                    }
                }
            else if(strcmp(bowled,"wd+w")==0){
                    team1.wickets++;
                    team2.bowlers[bowling].wicket++;
                    team1.scored++;
                team1.extras++;
                team2.bowlers[bowling].conceded++;
                j--;
                //stumping scenario
                     if(team1.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    j=7;
                    break;
                }
                printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    striker=next;
            }
            else if(strcmp(bowled,"w")==0){
                printf("Type of dismissal: (1-bowled,2-run out,3-caught,4-lbw,5-stumped,6-hit out): ");
                scanf("%d",&w);
                if(w==1){
                     team1.wickets++;
                     team2.bowlers[bowling].wicket++;
                     team1.batsmen[striker].ball++;
                     team2.bowlers[bowling].bowled++;
                      if(team1.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    break;
                }
                    printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    striker=next;
                }
                else if(w==3){
                     team1.wickets++;
                     team2.bowlers[bowling].wicket++;
                     team1.batsmen[striker].ball++;
                     team2.bowlers[bowling].bowled++;
                      if(team1.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    break;
                }
                    printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    striker=next;
                }
                else if(w==4){
                     team1.wickets++;
                     team2.bowlers[bowling].wicket++;
                     team1.batsmen[striker].ball++;
                     team2.bowlers[bowling].bowled++;
                      if(team1.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    break;
                }
                    printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    striker=next;
                }
                else if(w==5){
                     team1.wickets++;
                     team2.bowlers[bowling].wicket++;
                     team1.batsmen[striker].ball++;
                     team2.bowlers[bowling].bowled++;
                      if(team1.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    break;
                }
                    printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    striker=next;
                }
                else if(w==6){
                     team1.wickets++;
                     team2.bowlers[bowling].wicket++;
                     team1.batsmen[striker].ball++;
                     team2.bowlers[bowling].bowled++;
                      if(team1.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    break;
                }
                    printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    striker=next;
                }
                else if(w==2){
                     team1.wickets++;
                     team1.batsmen[striker].ball++;
                     team2.bowlers[bowling].bowled++;
                if(team1.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    break;
                }
                    printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    printf("Who was out (if striker then 1, if non striker then 2): ");
                    scanf("%d",&s);
                    if(s==1){
                    striker=next;
                    }
                    else if(s==2){
                    nonstriker=next;
                    }
                }
            }
        }
            x=striker;
            striker=nonstriker;
            nonstriker=x;
            //changing strike after finishing an over
            o1++;
    }

    printf("\n");
    printf("Score: %d/%d\n\n",team1.scored,team1.wickets);
    printf("%s need %d runs to win\n",team2.teamname,team1.scored+1);

    float reqrr=((float)team1.scored+1.0)/n;
    printf("Required Run Rate: %.2f\n\n",reqrr);



    printf("\n%s Innings: \n\n",team2.teamname);

    printf("Select striker: ");
    scanf("%d",&striker);
    printf("Select Non-striker: ");
    scanf("%d",&nonstriker);
    striker--;
    nonstriker--;

    for(i=0;i<n;i++){
            printf("Select bowler: ");
    scanf("%d",&bowling);
    bowling --;

        for(k=0;k<6;k++){
            printf("Over %d.%d: ",i,k+1);
            scanf("%s",bowled);

            if(strcmp(bowled,"wd")==0){
                team2.scored++;
                team2.extras++;
                team1.bowlers[bowling].conceded++;
                k--;
            if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"wd+1")==0){
                team2.scored=team2.scored+2;
                team2.extras=team2.extras+2;
                team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+2;
                k--;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
                if(team2.scored>team1.scored){
            i=n+1; //winning condition, breaks out of the loop
            break;
          }
            }
            else if(strcmp(bowled,"wd+2")==0){
                team2.scored=team2.scored+3;
                team2.extras=team2.extras+3;
                team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+3;
                k--;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"wd+3")==0){
                team2.scored=team2.scored+4;
                team2.extras=team2.extras+4;
                team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+4;
                k--;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"wd+4")==0){
                team2.scored=team2.scored+5;
                team2.extras=team2.extras+5;
                team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+5;
                k--;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"nb")==0){
                team2.scored=team2.scored+1;
                team2.extras=team2.extras+1;
                team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+1;
                team2.batsmen[striker].ball++;
                k--;
            if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"nb+1")==0){
                team2.scored=team2.scored+2;
                team2.extras=team2.extras+2;
                team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+2;
                team2.batsmen[striker].runs=team2.batsmen[striker].runs+1;
                team2.batsmen[striker].ball++;
                k--;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"nb+2")==0){
                team2.scored=team2.scored+3;
                team2.extras=team2.extras+1;
                team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+3;
                team2.batsmen[striker].runs=team2.batsmen[striker].runs+2;
                team2.batsmen[striker].ball++;
                k--;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"nb+3")==0){
                team2.scored=team2.scored+4;
                team2.extras=team2.extras+1;
                team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+4;
                team2.batsmen[striker].runs=team2.batsmen[striker].runs+3;
                team2.batsmen[striker].ball++;
                k--;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"nb+4")==0){
                team2.scored=team2.scored+5;
                team2.extras=team2.extras+1;
                team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+5;
                team2.batsmen[striker].runs=team2.batsmen[striker].runs+4;
                team2.batsmen[striker].ball++;
                k--;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"nb+5")==0){
                team2.scored=team2.scored+6;
                team2.extras=team2.extras+1;
                team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+6;
                team2.batsmen[striker].runs=team2.batsmen[striker].runs+5;
                team2.batsmen[striker].ball++;
                k--;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"nb+6")==0){
                team2.scored=team2.scored+7;
                team2.extras=team2.extras+1;
                team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+7;
                team2.batsmen[striker].runs=team2.batsmen[striker].runs+6;
                team2.batsmen[striker].ball++;
                team2.batsmen[striker].sixes++;
                k--;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"by")==0){
                team2.scored=team2.scored+1;
                team2.extras=team2.extras+1;
                team2.batsmen[striker].ball++;
                team1.bowlers[bowling].bowled++;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"by2")==0){
                team2.scored=team2.scored+2;
                team2.extras=team2.extras+2;
                team2.batsmen[striker].ball++;
                team1.bowlers[bowling].bowled++;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"by3")==0){
                team2.scored=team2.scored+3;
                team2.extras=team2.extras+3;
                team2.batsmen[striker].ball++;
                team1.bowlers[bowling].bowled++;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"by4")==0){
                team2.scored=team2.scored+4;
                team2.extras=team2.extras+4;
                team2.batsmen[striker].ball++;
                team1.bowlers[bowling].bowled++;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
           else if(strcmp(bowled,"by5")==0){
                team2.scored=team2.scored+5;
                team2.extras=team2.extras+5;
                team2.batsmen[striker].ball++;
                team1.bowlers[bowling].bowled++;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            }
            else if(strcmp(bowled,"lb")==0){
                team2.scored=team2.scored+1;
                team2.extras=team2.extras+1;
                team2.batsmen[striker].ball++;
                team1.bowlers[bowling].bowled++;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"lb2")==0){
                team2.scored=team2.scored+2;
                team2.extras=team2.extras+2;
                team2.batsmen[striker].ball++;
                team1.bowlers[bowling].bowled++;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"lb3")==0){
                team2.scored=team2.scored+3;
                team2.extras=team2.extras+3;
                team2.batsmen[striker].ball++;
                team1.bowlers[bowling].bowled++;
                x=striker;
                striker=nonstriker;
                nonstriker=x;if(team2.scored>team1.scored){
            i=n+1;
            break;
          }

            }
            else if(strcmp(bowled,"lb4")==0){
                team2.scored=team2.scored+4;
                team2.extras=team2.extras+4;
                team2.batsmen[striker].ball++;
                team1.bowlers[bowling].bowled++;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
               else if(strcmp(bowled,"0")==0){
                team2.batsmen[striker].ball++;
                team1.bowlers[bowling].bowled++;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"1")==0){
                team2.scored=team2.scored+1;
                team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+1;
                team2.batsmen[striker].runs=team2.batsmen[striker].runs+1;
                team2.batsmen[striker].ball++;
                team1.bowlers[bowling].bowled++;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"2")==0){
                team2.scored=team2.scored+2;
                team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+2;
                team2.batsmen[striker].runs=team2.batsmen[striker].runs+2;
                team2.batsmen[striker].ball++;
                team1.bowlers[bowling].bowled++;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"3")==0){
                team2.scored=team2.scored+3;
                team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+3;
                team2.batsmen[striker].runs=team2.batsmen[striker].runs+3;
                team2.batsmen[striker].ball++;
                team1.bowlers[bowling].bowled++;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"4")==0){
                team2.scored=team2.scored+4;
                team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+4;
                team2.batsmen[striker].runs=team2.batsmen[striker].runs+4;
                team2.batsmen[striker].ball++;
                team2.batsmen[striker].fours++;
                team1.bowlers[bowling].bowled++;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"5")==0){
                team2.scored=team2.scored+5;
                team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+5;
                team2.batsmen[striker].runs=team2.batsmen[striker].runs+5;
                team2.batsmen[striker].ball++;
                team1.bowlers[bowling].bowled++;
                x=striker;
                striker=nonstriker;
                nonstriker=x;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"6")==0){
                team2.scored=team2.scored+6;
                team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+6;
                team2.batsmen[striker].runs=team2.batsmen[striker].runs+6;
                team2.batsmen[striker].ball++;
                team1.bowlers[bowling].bowled++;
                team2.batsmen[striker].sixes++;
                if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"w+1")==0){
                     team2.wickets++;
                     team2.scored=team2.scored+1;
                     team2.batsmen[striker].ball++;
                     team2.batsmen[striker].runs=team2.batsmen[striker].runs+1;
                     team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+1;
                     team1.bowlers[bowling].bowled++;
                     x=striker;
                     striker=nonstriker;
                     nonstriker=x;
                      if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
                if(team2.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    break;
                }
                    printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    printf("Who was out (if striker then 1, if non striker then 2): ");
                    scanf("%d",&s);
                    if(s==1){
                    striker=next;
                    }
                    else if(s==2){
                    nonstriker=next;
                    }
                }
                else if(strcmp(bowled,"w+2")==0){
                     team2.wickets++;
                     team2.scored=team2.scored+2;
                     team2.batsmen[striker].ball++;
                     team2.batsmen[striker].runs=team2.batsmen[striker].runs+2;
                     team1.bowlers[bowling].conceded=team1.bowlers[bowling].conceded+2;
                     team1.bowlers[bowling].bowled++;
                      if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
                if(team2.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    break;
                }
                    printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    printf("Who was out (if striker then 1, if non striker then 2): ");
                    scanf("%d",&s);
                    if(s==1){
                    striker=next;
                    }
                    else if(s==2){
                    nonstriker=next;
                    }
                }
            else if(strcmp(bowled,"wd+w")==0){
                    team2.wickets++;
                    team1.bowlers[bowling].wicket++;
                    team2.scored++;
                team2.extras++;
                team1.bowlers[bowling].conceded++;
                k--;
                     if(team2.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    break;
                }
                printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    striker=next;
                    if(team2.scored>team1.scored){
            i=n+1;
            break;
          }
            }
            else if(strcmp(bowled,"w")==0){
                printf("Type of dismissal: (1-bowled,2-run out,3-caught,4-lbw,5-stumped,6-hit out): ");
                scanf("%d",&w);
                if(w==1){
                     team2.wickets++;
                     team1.bowlers[bowling].wicket++;
                     team2.batsmen[striker].ball++;
                     team1.bowlers[bowling].bowled++;
                      if(team2.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    break;
                }
                    printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    striker=next;
                }
                else if(w==3){
                     team2.wickets++;
                     team1.bowlers[bowling].wicket++;
                     team2.batsmen[striker].ball++;
                     team1.bowlers[bowling].bowled++;
                      if(team2.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    break;
                }
                    printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    striker=next;
                }
                else if(w==4){
                     team2.wickets++;
                     team1.bowlers[bowling].wicket++;
                     team2.batsmen[striker].ball++;
                     team1.bowlers[bowling].bowled++;
                      if(team2.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    break;
                }
                    printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    striker=next;
                }
                else if(w==5){
                     team2.wickets++;
                     team1.bowlers[bowling].wicket++;
                     team2.batsmen[striker].ball++;
                     team1.bowlers[bowling].bowled++;
                      if(team2.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    break;
                }
                    printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    striker=next;
                }
                else if(w==6){
                     team2.wickets++;
                     team1.bowlers[bowling].wicket++;
                     team2.batsmen[striker].ball++;
                     team1.bowlers[bowling].bowled++;
                      if(team2.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    break;
                }
                    printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    striker=next;
                }
                else if(w==2){
                     team2.wickets++;
                     team2.batsmen[striker].ball++;
                     team1.bowlers[bowling].bowled++;
                if(team2.wickets==10){
                    printf("\nALL OUT\n");
                    i=n+1;
                    break;
                }
                    printf("Enter next batsman: ");
                    scanf("%d",&next);
                    next--;
                    printf("Who was out (if striker then 1, if non striker then 2): ");
                    scanf("%d",&s);
                    if(s==1){
                    striker=next;
                    }
                    else if(s==2){
                    nonstriker=next;
                    }
                }
            }
        }
            x=striker;
            striker=nonstriker;
            nonstriker=x;
            o2++;
    }

    printf("\n\n");

    float rr1;
    float rr2;

    printf("\nSCORECARD\n");
    printf("==========\n\n");

    printf("%s Innings\n",team1.teamname);

    printf("Name\t\tRuns\tBalls\t4s\t6s\tSR\n");

    for(i=0;i<11;i++){
            if(team1.batsmen[i].ball==0){
                printf("%-10s\tD.N.B\n",team1.batsmen[i].name);
            }
            else{
                    team1.batsmen[i].strikerate=(float)team1.batsmen[i].runs/((float)team1.batsmen[i].ball);
        printf("%-10s\t%d\t%d\t%d\t%d\t%.2f\n",team1.batsmen[i].name,team1.batsmen[i].runs,team1.batsmen[i].ball,team1.batsmen[i].fours,team1.batsmen[i].sixes,team1.batsmen[i].strikerate*100);
    } //shows overs bowled, runs conceded, wickets taken and economy of the bowlers
}

for(i=0;i<11;i++){
    b1+=team2.bowlers[i].bowled;
    b1%=6; //for fixing the balls played in the last over of the innings
}

    printf("\nTOTAL: %d/%d",team1.scored,team1.wickets);
    printf("\nOvers: %d.%d\n",o1,b1);
    printf("Extras: %d\n",team1.extras);
    rr1=((float)team1.scored)/((float)o1+(float)b1/6.0);
    printf("Run Rate: %.2f\n\n",rr1);

    printf("Name\t\tO\tR\tW\tECON\n");
for(i=0;i<11;i++){
    if(team2.bowlers[i].bowled==0){
        continue;
    }
    int bl2=team2.bowlers[i].bowled;
    int ov2=bl2/6; //overs bowled by bowlers based on total balls bowled
    int r2=bl2%6;
    team2.bowlers[i].economy=(float)team2.bowlers[i].conceded/((float)team2.bowlers[i].bowled/6.0); //int has to be converted to float
    printf("%-10s\t%d.%d\t%d\t%d\t%.2f\n",team2.bowlers[i].name,ov2,r2,team2.bowlers[i].conceded,team2.bowlers[i].wicket,team2.bowlers[i].economy);
}

    printf("\n%s Innings\n",team2.teamname);

    printf("Name\t\tRuns\tBalls\t4s\t6s\tSR\n");

    for(i=0;i<11;i++){
            if(team2.batsmen[i].ball==0){
                printf("%-10s\tD.N.B\n",team2.batsmen[i].name);
            }
            else{
                    team2.batsmen[i].strikerate=(float)team2.batsmen[i].runs/((float)team2.batsmen[i].ball);
        printf("%-10s\t%d\t%d\t%d\t%d\t%.2f\n",team2.batsmen[i].name,team2.batsmen[i].runs,team2.batsmen[i].ball,team2.batsmen[i].fours,team2.batsmen[i].sixes,team2.batsmen[i].strikerate*100);
    }
}

for(i=0;i<11;i++){
    b2+=team1.bowlers[i].bowled;
    b2%=6;
}

if(team2.scored>team1.scored&&b2!=0){ //fixing the condition when team 2 wins on the last delivery of an over
    o2--; //for fixing the number of overs played while team 2 wins (breaking out of loop condition problem fixed here)
}

    printf("\nTOTAL: %d/%d",team2.scored,team2.wickets);
    printf("\nOvers: %d.%d\n",o2,b2);
    printf("Extras: %d\n",team2.extras);
    rr2=((float)team2.scored)/((float)o2+(float)b2/6.0);
    printf("Run Rate: %.2f\n\n",rr2);

    printf("Name\t\tO\tR\tW\tECON\n");
for(i=0;i<11;i++){
    if(team1.bowlers[i].bowled==0){
        continue;
    }
    int bl1=team1.bowlers[i].bowled;
    int ov1=bl1/6;
    int r1=bl1%6;
    team1.bowlers[i].economy=(float)team1.bowlers[i].conceded/((float)team1.bowlers[i].bowled/6.0);
    printf("%-10s\t%d.%d\t%d\t%d\t%.2f\n",team1.bowlers[i].name,ov1,r1,team1.bowlers[i].conceded,team1.bowlers[i].wicket,team1.bowlers[i].economy);
}

printf("\n\n");

printf("RESULT\n");
printf("==========\n\n");

if(team1.scored==team2.scored){
    printf("MATCH TIED\n");
}
else if(team1.scored>team2.scored){
    printf("%s BEAT %s BY %d RUNS\n",team1.teamname,team2.teamname,team1.scored-team2.scored);
}
else if(team1.scored<team2.scored){
    printf("%s BEAT %s BY %d WICKETS\n",team2.teamname,team1.teamname,10-team2.wickets);
}

return 0;

}
