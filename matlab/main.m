clear all;
routes=[5,90,405,520];
for i=1:4
    [result,mileSumArr,mileNumArr]=parseData(num2str(routes(i)), 2);
    drawSingle(result,['../graphs/',num2str(routes(i))]);
    
    results(i,:,:)=result(:,:);
    mileSumArrs(i,:)=mileSumArr(1,:)+mileSumArr(2,:);
    mileNumArrs(i,:)=mileNumArr(1,:)+mileNumArr(2,:);
    
end

for i=1:4
    result=parseData(num2str(routes(i)), 1);    
    results(i+4,:,:)=result(:,:);
end

for percentage=1:7
    for path=1:5
        result(percentage,path).speed=0;
        result(percentage,path).flow=0;
        result(percentage,path).density=0;
        result2(percentage,path).speed=0;
        result2(percentage,path).flow=0;
        result2(percentage,path).density=0;
    end
    for path=2:5
        for i=1:4
            result(percentage,path).speed = result(percentage,path).speed + ...
                results(i,percentage,path).speed * (mileSumArrs(i,path) / sum(mileSumArrs(:,path)));
            result(percentage,path).flow = result(percentage,path).flow + ...
                results(i,percentage,path).flow * (mileSumArrs(i,path) / sum(mileSumArrs(:,path)));
            result(percentage,path).density = result(percentage,path).density + ...
                results(i,percentage,path).density * (mileNumArrs(i,path) / sum(mileNumArrs(:,path)));
            result2(percentage,path).speed = result2(percentage,path).speed + ...
                results(i+4,percentage,path).speed * (mileSumArrs(i,path) / sum(mileSumArrs(:,path)));
            result2(percentage,path).flow = result2(percentage,path).flow + ...
                results(i+4,percentage,path).flow * (mileSumArrs(i,path) / sum(mileSumArrs(:,path)));
            result2(percentage,path).density = result2(percentage,path).density + ...
                results(i+4,percentage,path).density * (mileNumArrs(i,path) / sum(mileNumArrs(:,path)));
        end
    end
end
drawSingle(result,'../graphs/all');

drawBar(result,result2,'../graphs/all_bar');


