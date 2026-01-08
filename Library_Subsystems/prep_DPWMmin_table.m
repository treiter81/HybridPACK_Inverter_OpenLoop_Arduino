%clear 
%close all

doplot = 1;

steps = 1024;
angle = linspace(0,2*pi,steps+1);
angle = angle(1:end-1);

DPWMminDu = zeros(steps,1);
DPWMminDusec = zeros(steps,1);
DPWMminDuclamp = zeros(steps,1);

DPWMminDv = zeros(steps,1);
DPWMminDvsec = zeros(steps,1);
DPWMminDvclamp = zeros(steps,1);


DPWMminDw = zeros(steps,1);
DPWMminDwsec = zeros(steps,1);
DPWMminDwclamp = zeros(steps,1);



%%%%%%%%
%DPWMmin CALCULATION
%%%%%%%%
for i=1:length(angle)
    %M=0.5;
    M=1;
      
    if (rad2deg(angle(i))<120)
        DPWMminDusec(i) = -0.5;
        DPWMminDu(i) = -0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWMminDuclamp(i) = 0;
        
        DPWMminDvsec(i) = -0.5;
        DPWMminDv(i) = +0.5*(3^0.5 * M * sin(angle(i)) );
        DPWMminDvclamp(i) = 0;

        DPWMminDwsec(i) = -0.5;
        DPWMminDw(i) = 0;
        DPWMminDwclamp(i) = 2;


    elseif (rad2deg(angle(i))<240)
        DPWMminDusec(i) = -0.5;
        DPWMminDu(i) = 0;
        DPWMminDuclamp(i) = 2;
        
        DPWMminDvsec(i) = -0.5;
        DPWMminDv(i) = -0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWMminDvclamp(i) = 0;

        DPWMminDwsec(i) = -0.5;
        DPWMminDw(i) = +0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWMminDwclamp(i) = 0;

    else %till 360
        DPWMminDusec(i) = -0.5;
        DPWMminDu(i) = +0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWMminDuclamp(i) = 0;
        
        DPWMminDvsec(i) = -0.5;
        DPWMminDv(i) = 0;
        DPWMminDvclamp(i) = 2;

        DPWMminDwsec(i) = -0.5;
        DPWMminDw(i) = -0.5* (3^0.5 * M * sin(angle(i)) );
        DPWMminDwclamp(i) = 0;
    end


end


%%%%%%%%
%END DPWMmin CALCULATION
%%%%%%%%

fixedPointValue = fi(DPWMminDu, 1, 16, 14);
DPWMminDufi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWMminDusec, 1, 16, 14);
DPWMminDusecfi = storedInteger(fixedPointValue);

DPWMminDuclamp(DPWMminDuclamp==-1) = 2;
DPWMminDuclamp = uint8(DPWMminDuclamp);

%%
fixedPointValue = fi(DPWMminDv, 1, 16, 14);
DPWMminDvfi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWMminDvsec, 1, 16, 14);
DPWMminDvsecfi = storedInteger(fixedPointValue);

DPWMminDvclamp(DPWMminDvclamp==-1) = 2;
DPWMminDvclamp = uint8(DPWMminDvclamp);

%%

fixedPointValue = fi(DPWMminDw, 1, 16, 14);
DPWMminDwfi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWMminDwsec, 1, 16, 14);
DPWMminDwsecfi = storedInteger(fixedPointValue);

DPWMminDwclamp(DPWMminDwclamp==-1) = 2;
DPWMminDwclamp = uint8(DPWMminDwclamp);

if doplot ==1

%SVPWM.steps = steps;
%SVPWM.angle = angle;
%close all
figure
hold on
title('DPWMmin; double; real value')
%plot(angle,sec,'b')
plot(rad2deg(angle),DPWMminDusec + DPWMminDu,'b')
plot(rad2deg(angle),DPWMminDuclamp,'c--')

plot(rad2deg(angle),DPWMminDvsec + DPWMminDv,'r')
plot(rad2deg(angle),DPWMminDvclamp,'m--')

plot(rad2deg(angle),DPWMminDwsec + DPWMminDw,'g')
plot(rad2deg(angle),DPWMminDwclamp,'y--')


%plot(angle,SVPWM.Dv,'r')
%plot(angle,SVPWM.Dw,'g')
%hl(1) = legend;
grid on
% 
figure
hold on
title('DPWMmin; fixed point; stored integer')
plot(1:length(DPWMminDufi),DPWMminDufi,'b')
plot(1:length(DPWMminDvfi),DPWMminDvfi,'r')
plot(1:length(DPWMminDwfi),DPWMminDwfi,'g')
% hl(2) = legend;
% grid on
% 
% hl(1).String = {'U','V','W'};
% hl(2).String = {'U','V','W'};
% 
% 
end
clear fixedPointValue hl steps M i doplot