clear all;
D=readData('520');
DATA=[];DATAV=[];DATAP=[];DATAQ=[];
n=0;
datav=zeros(1,4);
datap=zeros(1,4);
dataq=zeros(1,4);
temp_datav=zeros(1,4);
temp_datap=zeros(1,4);
temp_dataq=zeros(1,4);
number=zeros(1,4);
quan=zeros(1,4);
length=zeros(1,4);
sump=zeros(1,4);
sumq=zeros(1,4);
v=0;p=0;q=0;
for percentage=1:1:7
for aord=1:2
DATA(:,:)=D(2,aord,percentage,:,:);
sza=size(DATA);
[B1,B2]=textread('../data/520.txt','%f%f%*[^\n]');
szb=size(B1);
for i=1:1:szb(1)
    B1(i,1)=B2(i,1)-B1(i,1);
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
    ave=v/(n);
    number(n-1)=number(n-1)+1;%number of sections
    quan(n-1)=quan(n-1)+ave*B1(DATA(i,1));
    length(n-1)=length(n-1)+B1(DATA(i,1));
    sump(n-1)=sump(n-1)+p;
    sumq(n-1)=sumq(n-1)+q;
for i=1:1:4
    if length(i)~=0
        temp_datav(aord,i)=quan(i)/length(i);
    end
    if number(i)~=0
        temp_datap(aord,i)=sump(i)/number(i);
        temp_dataq(aord,i)=sumq(i)/number(i);
    end
end
end
for i=1:4
datav=(temp_datav(1,i)+temp_datav(2,i))/2;
datap=(temp_datap(1,i)+temp_datap(2,i))/2;
dataq=(temp_dataq(1,i)+temp_dataq(2,i))/2;
end
DATAV=[DATAV;datav];
DATAP=[DATAP;datap];
DATAQ=[DATAQ;dataq];
end