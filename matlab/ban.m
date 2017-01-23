%************************************************* 
% ban.m
%*************************************************

function A=ban(route)
    type_list=['norm';'busy'];
    auto_list=['0.0';'0.1';'0.3';'0.5';'0.7';'0.9';'1.0'];
    for i=1:2
        for j=1:2
            for k=1:7
                if(j==1)
                    order='asc';
                else
                    order='desc';
                end
                str=[route,'_',type_list(i,:),'_',order,'_',auto_list(k,:),'_ban'];
                drawBan(['../output/',str,'.txt'],['../graphs/',str]);
            end
        end
    end
end