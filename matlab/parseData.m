function [result,mileSumArr,mileNumArr] = parseData( route,n )
    D=readData(route);
    result=[];

    for percentage=1:7

        mileSumArr=zeros(2,5);
        mileNumArr=zeros(2,5);
        
        for aord=1:2
            
            averageData=[];

            for i=1:5
                averageData(i).speed=0;
                averageData(i).flow=0;
                averageData(i).density=0;
            end
            
            
            DATA(:,:)=D(n,aord,percentage,:,:);

            sizeDATA=size(DATA);

            [milepostStart,milepostEnd]=textread(['../data/',route,'.txt'],'%f%f%*[^\n]');
            milepost=zeros(1,length(milepostStart));
            milepost(:)=milepostEnd(:)-milepostStart(:);
            
            
            mileSum=sum(milepost);
            mileSumPath=zeros(1,5);
            mileNumPath=zeros(1,5);
            
            milepostNo=1;
            milepostTotal=sizeDATA(1);

            for i=1:length(milepost)
                milepostData(i).speed=0;
                milepostData(i).flow=0;
                milepostData(i).density=0;
                milepostData(i).pathNum=0;            
            end

            for i=1:milepostTotal
                
                if(DATA(i,1)+1~=milepostNo)
                    break;
                end
                
                milepostData(milepostNo).pathNum=milepostData(milepostNo).pathNum+1;

                endFlag=0;
                if(i==milepostTotal)
                    endFlag=1;
                elseif(DATA(i+1,2)==0)
                    endFlag=1;
                end

                milepostData(milepostNo).speed=milepostData(milepostNo).speed+DATA(i,3);
                milepostData(milepostNo).flow=milepostData(milepostNo).flow+DATA(i,4);
                milepostData(milepostNo).density=milepostData(milepostNo).density+DATA(i,5);

                if(endFlag)
                    pathNum=milepostData(milepostNo).pathNum;
                    mileSumPath(pathNum)=mileSumPath(pathNum)+milepost(milepostNo);
                    mileNumPath(pathNum)=mileNumPath(pathNum)+1;
                    milepostNo=milepostNo+1;
                end
            end

            for i=1:length(milepost)
                pathNum=milepostData(i).pathNum;
                averageData(pathNum).speed = averageData(pathNum).speed + ...
                    milepostData(i).speed / pathNum * (milepost(i) / mileSumPath(pathNum));
                averageData(pathNum).flow = averageData(pathNum).flow + ...
                    milepostData(i).flow * (milepost(i) / mileSumPath(pathNum));
                averageData(pathNum).density = averageData(pathNum).density + ...
                    milepostData(i).density / mileNumPath(pathNum);
                
                averageData(1).speed = averageData(1).speed + ...
                    milepostData(i).speed / pathNum * (milepost(i) / mileSum);
                averageData(1).flow = averageData(1).flow + ...
                    milepostData(i).flow * (milepost(i) / mileSum);
                averageData(1).density = averageData(1).density + ...
                    milepostData(i).density / length(milepost);
            end
            for i=1:5
                mileSumArr(aord,i)=mileSumPath(i);
                mileNumArr(aord,i)=mileNumPath(i);
                if(aord==1)
                    result(percentage,i).speed=averageData(i).speed;
                    result(percentage,i).flow=averageData(i).flow;
                    result(percentage,i).density=averageData(i).density;
                else
                    if(mileSumArr(1,i) + mileSumArr(2,i)>0)
                        result(percentage,i).speed = ...
                            result(percentage,i).speed * ...
                            (mileSumArr(1,i) / (mileSumArr(1,i) + mileSumArr(2,i))) + ...
                            averageData(i).speed * ...
                            (mileSumArr(2,i) / (mileSumArr(1,i) + mileSumArr(2,i)));

                        result(percentage,i).flow = ...
                            result(percentage,i).flow * ...
                            (mileSumArr(1,i) / (mileSumArr(1,i) + mileSumArr(2,i))) + ...
                            averageData(i).flow * ...
                            (mileSumArr(2,i) / (mileSumArr(1,i) + mileSumArr(2,i)));
                    end
                    if(mileNumArr(1,i) + mileNumArr(2,i)>0)
                        result(percentage,i).density = ...
                            result(percentage,i).density * ...
                            (mileNumArr(1,i) / (mileNumArr(1,i) + mileNumArr(2,i))) + ...
                            averageData(i).density * ...
                            (mileNumArr(2,i) / (mileNumArr(1,i) + mileNumArr(2,i)));
                    end
                end
            end
        end

    end
    
    
    
end

