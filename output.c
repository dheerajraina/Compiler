#include<stdio.h>
int main(){
int age;
age = 60;
printf("%d\n", age);
int temp =90 + 10;
printf("%d\n", temp);
for (int i=0;i<10;i++){
int number = 10;
printf("%d\n", number);
}int number;
number = 90;
printf("%d\n", number);
if(number > age){printf("%d\n", number);
}else if(age > number){printf("%d\n", age);
}else{printf("%d\n", temp);
}return 0;
}