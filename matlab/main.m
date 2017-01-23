clear all;
D=readData('520');
DATA=[];DATAV=[];DATAP=[];DATAQ=[];
n=0;
datav=zeros(1,4);
datap=zeros(1,4);
dataq=zeros(1,4);
number=zeros(1,4);
quan=zeros(1,4);
length=zeros(1,4);
sump=zeros(1,4);
sumq=zeros(1,4);
v=0;p=0;q=0;
for ii=1:1:10
for i=1:2
    A(:,:)=D(2,i,ii,:,:);
    DATA=[DATA;A];
end
sza=size(DATA);
for i=sza(1)/2+1:1:sza(1)
    DATA(i)=DATA(i-sza(1)/2);
end
[B1,B2]=textread('../data/520.txt','%f%f%*[^\n]');
B1=[B1;B1];B2=[B2;B2];
szb=size(B1);
for i=1:1:szb(1)
    B1(i)=B2(i)-B1(i);
end
for i=1:1:sza(1)
    if DATA(i,2)==0 && i~=1;
        ave=v/n;
        number(n-1)=number(n-1)+1;%number of sections
        quan(n-1)=quan(n-1)+ave*B1(DATA(i,1));
        length(n-1)=length(n-1)+B1(DATA(i,1));
        sump(n-1)=sump(n-1)+p;
        sumq(n-1)=sumq(n-1)+q;
        n=0;v=0;
    end
    n=n+1;
    v=v+DATA(i,3);
    q=q+DATA(i,4);
    p=p+DATA(i,5);
end
for i=1:1:4
    if length(i)~=0
        datav(i)=quan(i)/length(i);
    end
    if number(i)~=0
        datap(i)=sump(i)/number(i);
        dataq(i)=sumq(i)/number(i);
    end
end
DATAV=[DATAV;datav];
DATAP=[DATAP;datap];
DATAQ=[DATAQ;dataq];
end