%clear 
%close all

doplot = 1;

steps = 1024;
angle = linspace(0,2*pi,steps+1);
angle = angle(1:end-1);

DPWMmaxDu = zeros(steps,1);
DPWMmaxDusec = zeros(steps,1);
DPWMmaxDuclamp = zeros(steps,1);

DPWMmaxDv = zeros(steps,1);
DPWMmaxDvsec = zeros(steps,1);
DPWMmaxDvclamp = zeros(steps,1);


DPWMmaxDw = zeros(steps,1);
DPWMmaxDwsec = zeros(steps,1);
DPWMmaxDwclamp = zeros(steps,1);



%%%%%%%%
%DPWMmax CALCULATION
%%%%%%%%
for i=1:length(angle)
    %M=0.5;
    M=1;
      
    if (rad2deg(angle(i))<60)
        DPWMmaxDusec(i) = +0.5;
        DPWMmaxDu(i) = 0;
        DPWMmaxDuclamp(i) = 1;
        
        DPWMmaxDvsec(i) = +0.5;
        DPWMmaxDv(i) = -0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWMmaxDvclamp(i) = 0;

        DPWMmaxDwsec(i) = +0.5;
        DPWMmaxDw(i) = +0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWMmaxDwclamp(i) = 0;


    elseif (rad2deg(angle(i))<180)
        DPWMmaxDusec(i) = +0.5;
        DPWMmaxDu(i) = +0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWMmaxDuclamp(i) = 0;
        
        DPWMmaxDvsec(i) = +0.5;
        DPWMmaxDv(i) = 0;
        DPWMmaxDvclamp(i) = 1;

        DPWMmaxDwsec(i) = +0.5;
        DPWMmaxDw(i) = -0.5*(3^0.5 * M * sin(angle(i)));
        DPWMmaxDwclamp(i) = 0;


    elseif (rad2deg(angle(i))<300)
        DPWMmaxDusec(i) = +0.5;
        DPWMmaxDu(i) = -0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWMmaxDuclamp(i) = 0;
        
        DPWMmaxDvsec(i) = +0.5;
        DPWMmaxDv(i) = +0.5*(3^0.5 * M * sin(angle(i)));
        DPWMmaxDvclamp(i) = 0;

        DPWMmaxDwsec(i) = +0.5;
        DPWMmaxDw(i) = 0;
        DPWMmaxDwclamp(i) = 1;
   

    else %till 360
        DPWMmaxDusec(i) = +0.5;
        DPWMmaxDu(i) = 0;
        DPWMmaxDuclamp(i) = 1;
        
        DPWMmaxDvsec(i) = +0.5;
        DPWMmaxDv(i) = -0.5* (3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWMmaxDvclamp(i) = 0;

        DPWMmaxDwsec(i) = +0.5;
        DPWMmaxDw(i) = +0.5* (3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWMmaxDwclamp(i) = 0;

    end


end


%%%%%%%%
%END DPWMmax CALCULATION
%%%%%%%%

fixedPointValue = fi(DPWMmaxDu, 1, 16, 14);
DPWMmaxDufi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWMmaxDusec, 1, 16, 14);
DPWMmaxDusecfi = storedInteger(fixedPointValue);

DPWMmaxDuclamp(DPWMmaxDuclamp==-1) = 2;
DPWMmaxDuclamp = uint8(DPWMmaxDuclamp);

%%
fixedPointValue = fi(DPWMmaxDv, 1, 16, 14);
DPWMmaxDvfi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWMmaxDvsec, 1, 16, 14);
DPWMmaxDvsecfi = storedInteger(fixedPointValue);

DPWMmaxDvclamp(DPWMmaxDvclamp==-1) = 2;
DPWMmaxDvclamp = uint8(DPWMmaxDvclamp);

%%

fixedPointValue = fi(DPWMmaxDw, 1, 16, 14);
DPWMmaxDwfi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWMmaxDwsec, 1, 16, 14);
DPWMmaxDwsecfi = storedInteger(fixedPointValue);

DPWMmaxDwclamp(DPWMmaxDwclamp==-1) = 2;
DPWMmaxDwclamp = uint8(DPWMmaxDwclamp);

if doplot ==1

%SVPWM.steps = steps;
%SVPWM.angle = angle;
%close all
figure(1)
hold on
title('DPWMmax; double; real value')
%plot(angle,sec,'b')
plot(rad2deg(angle),DPWMmaxDusec + DPWMmaxDu,'b')
plot(rad2deg(angle),DPWMmaxDuclamp,'c--')

plot(rad2deg(angle),DPWMmaxDvsec + DPWMmaxDv,'r')
plot(rad2deg(angle),DPWMmaxDvclamp,'m--')

plot(rad2deg(angle),DPWMmaxDwsec + DPWMmaxDw,'g')
plot(rad2deg(angle),DPWMmaxDwclamp,'y--')


%plot(angle,SVPWM.Dv,'r')
%plot(angle,SVPWM.Dw,'g')
%hl(1) = legend;
grid on
% 
figure
hold on
title('DPWMmax; fixed point; stored integer')
plot(1:length(DPWMmaxDufi),DPWMmaxDufi,'b')
plot(1:length(DPWMmaxDvfi),DPWMmaxDvfi,'r')
plot(1:length(DPWMmaxDwfi),DPWMmaxDwfi,'g')
% hl(2) = legend;
% grid on
% 
% hl(1).String = {'U','V','W'};
% hl(2).String = {'U','V','W'};
% 
% 
end
clear fixedPointValue hl steps M i doplot