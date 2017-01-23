evalMatrix=zeros(5,3);
standMatrix=zeros(5,3);
final=[];
for lane=1:1:7
for i=1:1:5
    evalMatrix(i,1)=result(lane, i).speed;
    evalMatrix(i,2)=result(lane, i).flow;
    evalMatrix(i,3)=result(lane, i).density;
end

for j=1:1:3
    minimal(j)=min(evalMatrix(2:5,j));
    maximal(j)=max(evalMatrix(2:5,j));
end
ideal=[maximal(1),maximal(2),minimal(3)];
for i=2:1:5
    for j=1:1:3
        standMatrix(i,j)=abs(evalMatrix(i,j)-ideal(j))/(maximal(j)-minimal(j));
    end
end

for j=1:1:3
    x(j)=sum(standMatrix(2:5,j)')/4;
    s(j)=std(standMatrix(2:5,j));
    w(j)=x(j)/s(j);
end
for j=1:1:3
    a(j)=w(j)/sum(w(:));
end
standMatrix=standMatrix';
summ=zeros(1,5);
sumsum=0;
for j=2:1:5
    for i=1:1:3
        sumsum=sumsum+a(i)*standMatrix(i,j);
    end
    summ(j)=sumsum;
end
final=[final;summ(1,2:5)];
end


