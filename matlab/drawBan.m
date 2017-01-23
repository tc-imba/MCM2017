%************************************************* 
% drawBan.m
%*************************************************

function drawBan(input,output)
    clf;
    fid=fopen(input);
    if(fid<0)
        return;
    end
    n=fscanf(fid,'%d\n', 1);
    for i=1:n
        m=fscanf(fid,'%d\n', 1);
        time=zeros(1,m);
        distance=zeros(1,m);
        for j=1:m
            time(j)=fscanf(fid,'%f', 1);
            distance(j)=fscanf(fid,'%f\n', 1);
        end
        hold on;
        plot(time,distance,'black');
        hold off;
    end
    fclose(fid);
    xlim([360,1080]);
    ylim([0.5,5.5]);
    set(gca,'YTickLabel',0.0:0.5:5.0);
    set(gca,'XTick',360:90:1080);
    set(gca,'XTickLabel',0:90:720);
    xlabel('Time (s)');
    ylabel('Location (mile)');
    saveas(gcf,[output,'.png']);
end