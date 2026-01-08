%clear 
%close all

doplot = 1;

steps = 1024;
angle = linspace(0,2*pi,steps+1);
angle = angle(1:end-1);

DPWM0Du = zeros(steps,1);
DPWM0Dusec = zeros(steps,1);
DPWM0Duclamp = zeros(steps,1);

DPWM0Dv = zeros(steps,1);
DPWM0Dvsec = zeros(steps,1);
DPWM0Dvclamp = zeros(steps,1);


DPWM0Dw = zeros(steps,1);
DPWM0Dwsec = zeros(steps,1);
DPWM0Dwclamp = zeros(steps,1);



%%%%%%%%
%DPWM0 CALCULATION
%%%%%%%%
for i=1:length(angle)
    %M=0.5;
    M=1;
      
    if (rad2deg(angle(i))<60)
        DPWM0Dusec(i) = -0.5;
        DPWM0Du(i) = -0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM0Duclamp(i) = 0;
        
        DPWM0Dvsec(i) = -0.5;
        DPWM0Dv(i) = +0.5*(3^0.5 * M * sin(angle(i)) );
        DPWM0Dvclamp(i) = 0;

        DPWM0Dwsec(i) = -0.5;
        DPWM0Dw(i) = 0;
        DPWM0Dwclamp(i) = 2;


    elseif (rad2deg(angle(i))<120)
        DPWM0Dusec(i) = +0.5;
        DPWM0Du(i) = +0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM0Duclamp(i) = 0;
        
        DPWM0Dvsec(i) = +0.5;
        DPWM0Dv(i) = 0;
        DPWM0Dvclamp(i) = 1;

        DPWM0Dwsec(i) = +0.5;
        DPWM0Dw(i) = -0.5*(3^0.5 * M * sin(angle(i)) );
        DPWM0Dwclamp(i) = 0;


    elseif (rad2deg(angle(i))<180)
        DPWM0Dusec(i) = -0.5;
        DPWM0Du(i) = 0;
        DPWM0Duclamp(i) = 2;
        
        DPWM0Dvsec(i) = -0.5;
        DPWM0Dv(i) = -0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM0Dvclamp(i) = 0;

        DPWM0Dwsec(i) = -0.5;
        DPWM0Dw(i) = +0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM0Dwclamp(i) = 0;
    
    elseif (rad2deg(angle(i))<240)
        DPWM0Dusec(i) = +0.5;
        DPWM0Du(i) = -0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM0Duclamp(i) = 0;
        
        DPWM0Dvsec(i) = +0.5;
        DPWM0Dv(i) = +0.5*(3^0.5 * M * sin(angle(i)) );
        DPWM0Dvclamp(i) = 0;

        DPWM0Dwsec(i) = +0.5;
        DPWM0Dw(i) = 0;
        DPWM0Dwclamp(i) = 1;

    elseif (rad2deg(angle(i))<300)
        DPWM0Dusec(i) = -0.5;
        DPWM0Du(i) = +0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)));
        DPWM0Duclamp(i) = 0;
        
        DPWM0Dvsec(i) = -0.5;
        DPWM0Dv(i) = 0;
        DPWM0Dvclamp(i) = 2;

        DPWM0Dwsec(i) = -0.5;
        DPWM0Dw(i) = -0.5*(3^0.5 * M * sin(angle(i)) );
        DPWM0Dwclamp(i) = 0;
    
    else %till 360
        DPWM0Dusec(i) = +0.5;
        DPWM0Du(i) = 0;
        DPWM0Duclamp(i) = 1;
        
        DPWM0Dvsec(i) = +0.5;
        DPWM0Dv(i) = -0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)));
        DPWM0Dvclamp(i) = 0;

        DPWM0Dwsec(i) = +0.5;
        DPWM0Dw(i) = +0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)));
        DPWM0Dwclamp(i) = 0;

    end
end


%clear sector Du Dv Dw T0 T1 T2
%%%%%%%%
%END DPWM0 CALCULATION
%%%%%%%%

fixedPointValue = fi(DPWM0Du, 1, 16, 14);
DPWM0Dufi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWM0Dusec, 1, 16, 14);
DPWM0Dusecfi = storedInteger(fixedPointValue);

DPWM0Duclamp(DPWM0Duclamp==-1) = 2;
DPWM0Duclamp = uint8(DPWM0Duclamp);

%%
fixedPointValue = fi(DPWM0Dv, 1, 16, 14);
DPWM0Dvfi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWM0Dvsec, 1, 16, 14);
DPWM0Dvsecfi = storedInteger(fixedPointValue);

DPWM0Dvclamp(DPWM0Dvclamp==-1) = 2;
DPWM0Dvclamp = uint8(DPWM0Dvclamp);

%%

fixedPointValue = fi(DPWM0Dw, 1, 16, 14);
DPWM0Dwfi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWM0Dwsec, 1, 16, 14);
DPWM0Dwsecfi = storedInteger(fixedPointValue);

DPWM0Dwclamp(DPWM0Dwclamp==-1) = 2;
DPWM0Dwclamp = uint8(DPWM0Dwclamp);


if doplot ==1
%SVPWM.steps = steps;
%SVPWM.angle = angle;
%close all
figure(1)
hold on
title('DPWM0; double; real value')
%plot(angle,sec,'b')
plot(rad2deg(angle),DPWM0Dusec + DPWM0Du,'b')
plot(rad2deg(angle),DPWM0Duclamp,'c--')

plot(rad2deg(angle),DPWM0Dvsec + DPWM0Dv,'r')
plot(rad2deg(angle),DPWM0Dvclamp,'m--')

plot(rad2deg(angle),DPWM0Dwsec + DPWM0Dw,'g')
plot(rad2deg(angle),DPWM0Dwclamp,'y--')


%plot(angle,SVPWM.Dv,'r')
%plot(angle,SVPWM.Dw,'g')
%hl(1) = legend;
grid on
% 
figure
hold on
title('DPWM0; fixed point; stored integer')
plot(1:length(DPWM0Dufi),DPWM0Dufi,'b')
plot(1:length(DPWM0Dvfi),DPWM0Dvfi,'r')
plot(1:length(DPWM0Dwfi),DPWM0Dwfi,'g')
% hl(2) = legend;
% grid on
% 
% hl(1).String = {'U','V','W'};
% hl(2).String = {'U','V','W'};
% 
% 
end
clear fixedPointValue hl steps M i doplot