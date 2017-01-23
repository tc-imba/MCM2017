%************************************************* 
% drawBar.m
%*************************************************

function drawBar(result,result2,filepath)
    xValue=[0,10,30,50,70,90,100];
    yValue=zeros(6,7);
    for i=1:3
        figure(i);
        clf;
        xlim([-10 110]);
        set(gca,'XTick',0:10:100);
    end
    for pathNum=2:5
        for percentage=1:7
            yValue(1,percentage)=result(percentage,pathNum).speed;
            yValue(2,percentage)=result2(percentage,pathNum).speed;
            yValue(3,percentage)=result(percentage,pathNum).flow;
            yValue(4,percentage)=result2(percentage,pathNum).flow;
            yValue(5,percentage)=result(percentage,pathNum).density;
            yValue(6,percentage)=result2(percentage,pathNum).density;
        end
        for i=1:3
            figure(i);
            if(sum(yValue(i,:))>0)
                hold on;
                yBar=yValue(2*i-1:2*i,:);
                bar(xValue,yBar');
                hold off;
            end
        end
    end
    for i=1:3
        figure(i);
        legend('busy','normal');
        % @TODO Add more information
        
        
        saveas(gcf,[filepath,'_',num2str(i),'.png']);
    end

end

