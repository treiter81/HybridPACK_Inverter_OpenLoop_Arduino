%clear 
%close all
doplot = 1;

steps = 1024;
angle = linspace(0,2*pi,steps+1);
angle = angle(1:end-1);

DPWM2Du = zeros(steps,1);
DPWM2Dusec = zeros(steps,1);
DPWM2Duclamp = zeros(steps,1);

DPWM2Dv = zeros(steps,1);
DPWM2Dvsec = zeros(steps,1);
DPWM2Dvclamp = zeros(steps,1);


DPWM2Dw = zeros(steps,1);
DPWM2Dwsec = zeros(steps,1);
DPWM2Dwclamp = zeros(steps,1);



%%%%%%%%
%DPWM2 CALCULATION
%%%%%%%%
for i=1:length(angle)
    %M=0.5;
    M=1;
      
    if (rad2deg(angle(i))<60)
        DPWM2Dusec(i) = +0.5;
        DPWM2Du(i) = 0;
        DPWM2Duclamp(i) = 1;
        
        DPWM2Dvsec(i) = +0.5;
        DPWM2Dv(i) = -0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM2Dvclamp(i) = 0;

        DPWM2Dwsec(i) = +0.5;
        DPWM2Dw(i) = +0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM2Dwclamp(i) = 0;


    elseif (rad2deg(angle(i))<120)
        DPWM2Dusec(i) = -0.5;
        DPWM2Du(i) = -0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM2Duclamp(i) = 0;
        
        DPWM2Dvsec(i) = -0.5;
        DPWM2Dv(i) = +0.5*(3^0.5 * M * sin(angle(i)) );
        DPWM2Dvclamp(i) = 0;

        DPWM2Dwsec(i) = -0.5;
        DPWM2Dw(i) = 0;
        DPWM2Dwclamp(i) = 2;


    elseif (rad2deg(angle(i))<180)
        DPWM2Dusec(i) = +0.5;
        DPWM2Du(i) = +0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM2Duclamp(i) = 0;
        
        DPWM2Dvsec(i) = +0.5;
        DPWM2Dv(i) = 0;
        DPWM2Dvclamp(i) = +1;

        DPWM2Dwsec(i) = +0.5;
        DPWM2Dw(i) = -0.5*(3^0.5 * M * sin(angle(i)) );
        DPWM2Dwclamp(i) = 0;
    
    elseif (rad2deg(angle(i))<240)
        DPWM2Dusec(i) = -0.5;
        DPWM2Du(i) = 0;
        DPWM2Duclamp(i) = 2;
        
        DPWM2Dvsec(i) = -0.5;
        DPWM2Dv(i) = -0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM2Dvclamp(i) = 0;

        DPWM2Dwsec(i) = -0.5;
        DPWM2Dw(i) = +0.5* (3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM2Dwclamp(i) = 0;

    elseif (rad2deg(angle(i))<300)
        DPWM2Dusec(i) = +0.5;
        DPWM2Du(i) = -0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)));
        DPWM2Duclamp(i) = 0;
        
        DPWM2Dvsec(i) = +0.5;
        DPWM2Dv(i) = +0.5* (3^0.5 * M * sin(angle(i)) );
        DPWM2Dvclamp(i) = 0;

        DPWM2Dwsec(i) = +0.5;
        DPWM2Dw(i) = 0;
        DPWM2Dwclamp(i) = +1;
    
    else %till 360
        DPWM2Dusec(i) = -0.5;
        DPWM2Du(i) = +0.5* (3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM2Duclamp(i) = 0;
        
        DPWM2Dvsec(i) = -0.5;
        DPWM2Dv(i) = 0;
        DPWM2Dvclamp(i) = 2;

        DPWM2Dwsec(i) = -0.5;
        DPWM2Dw(i) = -0.5* (3^0.5 * M * sin(angle(i)) );
        DPWM2Dwclamp(i) = 0;

    end


end


%clear sector Du Dv Dw T0 T1 T2
%%%%%%%%
%END DPWM2 CALCULATION
%%%%%%%%

fixedPointValue = fi(DPWM2Du, 1, 16, 14);
DPWM2Dufi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWM2Dusec, 1, 16, 14);
DPWM2Dusecfi = storedInteger(fixedPointValue);

DPWM2Duclamp(DPWM2Duclamp==-1) = 2;
DPWM2Duclamp = uint8(DPWM2Duclamp);

%%
fixedPointValue = fi(DPWM2Dv, 1, 16, 14);
DPWM2Dvfi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWM2Dvsec, 1, 16, 14);
DPWM2Dvsecfi = storedInteger(fixedPointValue);

DPWM2Dvclamp(DPWM2Dvclamp==-1) = 2;
DPWM2Dvclamp = uint8(DPWM2Dvclamp);

%%

fixedPointValue = fi(DPWM2Dw, 1, 16, 14);
DPWM2Dwfi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWM2Dwsec, 1, 16, 14);
DPWM2Dwsecfi = storedInteger(fixedPointValue);

DPWM2Dwclamp(DPWM2Dwclamp==-1) = 2;
DPWM2Dwclamp = uint8(DPWM2Dwclamp);


if doplot ==1
%SVPWM.steps = steps;
%SVPWM.angle = angle;
%close all
figure(1)
hold on
title('DPWM2; double; real value')
%plot(angle,sec,'b')
plot(rad2deg(angle),DPWM2Dusec + DPWM2Du,'b')
plot(rad2deg(angle),DPWM2Duclamp,'c--')

plot(rad2deg(angle),DPWM2Dvsec + DPWM2Dv,'r')
plot(rad2deg(angle),DPWM2Dvclamp,'m--')

plot(rad2deg(angle),DPWM2Dwsec + DPWM2Dw,'g')
plot(rad2deg(angle),DPWM2Dwclamp,'y--')


%plot(angle,SVPWM.Dv,'r')
%plot(angle,SVPWM.Dw,'g')
%hl(1) = legend;
grid on
% 
figure
hold on
title('DPWM2; fixed point; stored integer')
plot(1:length(DPWM2Dufi),DPWM2Dufi,'b')
plot(1:length(DPWM2Dvfi),DPWM2Dvfi,'r')
plot(1:length(DPWM2Dwfi),DPWM2Dwfi,'g')
% hl(2) = legend;
% grid on
% 
% hl(1).String = {'U','V','W'};
% hl(2).String = {'U','V','W'};
% 
% 
end
clear fixedPointValue hl steps M i doplot