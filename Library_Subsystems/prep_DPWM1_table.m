%clear 
%close all

doplot = 1;

steps = 1024;
angle = linspace(0,2*pi,steps+1);
angle = angle(1:end-1);

DPWM1Du = zeros(steps,1);
DPWM1Dusec = zeros(steps,1);
DPWM1Duclamp = zeros(steps,1);

DPWM1Dv = zeros(steps,1);
DPWM1Dvsec = zeros(steps,1);
DPWM1Dvclamp = zeros(steps,1);


DPWM1Dw = zeros(steps,1);
DPWM1Dwsec = zeros(steps,1);
DPWM1Dwclamp = zeros(steps,1);



%%%%%%%%
%DPWM1 CALCULATION
%%%%%%%%
for i=1:length(angle)
    %M=0.5;
    M=1;
      
    if (rad2deg(angle(i))<30)
        DPWM1Dusec(i) = +0.5;
        DPWM1Du(i) = 0;
        DPWM1Duclamp(i) = 1;
        
        DPWM1Dvsec(i) = +0.5;
        DPWM1Dv(i) = -0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM1Dvclamp(i) = 0;

        DPWM1Dwsec(i) = +0.5;
        DPWM1Dw(i) = +0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM1Dwclamp(i) = 0;


    elseif (rad2deg(angle(i))<90)
        DPWM1Dusec(i) = -0.5;
        DPWM1Du(i) = -0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM1Duclamp(i) = 0;
        
        DPWM1Dvsec(i) = -0.5;
        DPWM1Dv(i) = +0.5*(3^0.5 * M * sin(angle(i)) );
        DPWM1Dvclamp(i) = 0;

        DPWM1Dwsec(i) = -0.5;
        DPWM1Dw(i) = 0;
        DPWM1Dwclamp(i) = 2;


    elseif (rad2deg(angle(i))<150)
        DPWM1Dusec(i) = +0.5;
        DPWM1Du(i) = +0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM1Duclamp(i) = 0;
        
        DPWM1Dvsec(i) = +0.5;
        DPWM1Dv(i) = 0;
        DPWM1Dvclamp(i) = +1;

        DPWM1Dwsec(i) = +0.5;
        DPWM1Dw(i) = -0.5*(3^0.5 * M * sin(angle(i)) );
        DPWM1Dwclamp(i) = 0;
    
    elseif (rad2deg(angle(i))<210)
        DPWM1Dusec(i) = -0.5;
        DPWM1Du(i) = 0;
        DPWM1Duclamp(i) = 2;
        
        DPWM1Dvsec(i) = -0.5;
        DPWM1Dv(i) = -0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM1Dvclamp(i) = 0;

        DPWM1Dwsec(i) = -0.5;
        DPWM1Dw(i) = +0.5* (3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM1Dwclamp(i) = 0;

    elseif (rad2deg(angle(i))<270)
        DPWM1Dusec(i) = +0.5;
        DPWM1Du(i) = -0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)));
        DPWM1Duclamp(i) = 0;
        
        DPWM1Dvsec(i) = +0.5;
        DPWM1Dv(i) = +0.5* (3^0.5 * M * sin(angle(i)) );
        DPWM1Dvclamp(i) = 0;

        DPWM1Dwsec(i) = +0.5;
        DPWM1Dw(i) = 0;
        DPWM1Dwclamp(i) = +1;
    
    elseif (rad2deg(angle(i))<330)
        DPWM1Dusec(i) = -0.5;
        DPWM1Du(i) = +0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );  
        DPWM1Duclamp(i) = 0;
        
        DPWM1Dvsec(i) = -0.5;
        DPWM1Dv(i) = 0;
        DPWM1Dvclamp(i) = 2;

        DPWM1Dwsec(i) = -0.5;
        DPWM1Dw(i) =  -0.5* (3^0.5 * M * sin(angle(i)) );
        DPWM1Dwclamp(i) = 0;

    else %till 360
        DPWM1Dusec(i) = +0.5;
        DPWM1Du(i) = 0;
        DPWM1Duclamp(i) = 1;
        
        DPWM1Dvsec(i) = +0.5;
        DPWM1Dv(i) = -0.5* (3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM1Dvclamp(i) = 0;

        DPWM1Dwsec(i) = +0.5;
        DPWM1Dw(i) = +0.5* (3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM1Dwclamp(i) = 0;

    end


end


%%%%%%%%
%END DPWM1 CALCULATION
%%%%%%%%

fixedPointValue = fi(DPWM1Du, 1, 16, 14);
DPWM1Dufi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWM1Dusec, 1, 16, 14);
DPWM1Dusecfi = storedInteger(fixedPointValue);

DPWM1Duclamp(DPWM1Duclamp==-1) = 2;
DPWM1Duclamp = uint8(DPWM1Duclamp);

%%
fixedPointValue = fi(DPWM1Dv, 1, 16, 14);
DPWM1Dvfi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWM1Dvsec, 1, 16, 14);
DPWM1Dvsecfi = storedInteger(fixedPointValue);

DPWM1Dvclamp(DPWM1Dvclamp==-1) = 2;
DPWM1Dvclamp = uint8(DPWM1Dvclamp);

%%

fixedPointValue = fi(DPWM1Dw, 1, 16, 14);
DPWM1Dwfi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWM1Dwsec, 1, 16, 14);
DPWM1Dwsecfi = storedInteger(fixedPointValue);

DPWM1Dwclamp(DPWM1Dwclamp==-1) = 2;
DPWM1Dwclamp = uint8(DPWM1Dwclamp);

if doplot ==1

%SVPWM.steps = steps;
%SVPWM.angle = angle;
%close all
figure
hold on
title('DPWM1; double; real value')
%plot(angle,sec,'b')
plot(rad2deg(angle),DPWM1Dusec + DPWM1Du,'b')
plot(rad2deg(angle),DPWM1Duclamp,'c--')

plot(rad2deg(angle),DPWM1Dvsec + DPWM1Dv,'r')
plot(rad2deg(angle),DPWM1Dvclamp,'m--')

plot(rad2deg(angle),DPWM1Dwsec + DPWM1Dw,'g')
plot(rad2deg(angle),DPWM1Dwclamp,'y--')


%plot(angle,SVPWM.Dv,'r')
%plot(angle,SVPWM.Dw,'g')
%hl(1) = legend;
grid on
% 
figure
hold on
title('DPWM1; fixed point; stored integer')
plot(1:length(DPWM1Dufi),DPWM1Dufi,'b')
plot(1:length(DPWM1Dvfi),DPWM1Dvfi,'r')
plot(1:length(DPWM1Dwfi),DPWM1Dwfi,'g')
% hl(2) = legend;
% grid on
% 
% hl(1).String = {'U','V','W'};
% hl(2).String = {'U','V','W'};
% 
% 
end
clear fixedPointValue hl steps M i doplot