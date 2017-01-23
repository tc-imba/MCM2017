function drawSingle(result,filepath)
    for i=1:3
        figure(i);
        clf;
    end
    xValue=[0,10,30,50,70,90,100];
    yValue=zeros(3,7);
    ylegend=[];
    for pathNum=2:5
        for percentage=1:7
            yValue(1,percentage)=result(percentage,pathNum).speed;
            yValue(2,percentage)=result(percentage,pathNum).flow;
            yValue(3,percentage)=result(percentage,pathNum).density;
        end
        if(sum(yValue(1,:))>0)
            ylegend=[ylegend;num2str(pathNum)];
        end
        for i=1:3
            figure(i);
            if(sum(yValue(i,:))>0)
                hold on;
                plot(xValue,yValue(i,:),'-*');

                hold off;
            end
        end
    end
    for i=1:3
        figure(i);
        legend(ylegend);
        % @TODO Add more information
        
        
        saveas(gcf,[filepath,'_',num2str(i),'.png']);
    end

end

