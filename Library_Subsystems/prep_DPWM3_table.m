%clear 
%close all

doplot = 1;

steps = 1024;
angle = linspace(0,2*pi,steps+1);
angle = angle(1:end-1);

DPWM3Du = zeros(steps,1);
DPWM3Dusec = zeros(steps,1);
DPWM3Duclamp = zeros(steps,1);

DPWM3Dv = zeros(steps,1);
DPWM3Dvsec = zeros(steps,1);
DPWM3Dvclamp = zeros(steps,1);


DPWM3Dw = zeros(steps,1);
DPWM3Dwsec = zeros(steps,1);
DPWM3Dwclamp = zeros(steps,1);



%%%%%%%%
%DPWM3 CALCULATION
%%%%%%%%
for i=1:length(angle)
    %M=0.5;
    M=1;
      
    if (rad2deg(angle(i))<30)
        DPWM3Dusec(i) = -0.5;
        DPWM3Du(i) = -0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM3Duclamp(i) = 0;
        
        DPWM3Dvsec(i) = -0.5;
        DPWM3Dv(i) = +0.5*(3^0.5 * M * sin(angle(i)) );
        DPWM3Dvclamp(i) = 0;

        DPWM3Dwsec(i) = -0.5;
        DPWM3Dw(i) = 0;
        DPWM3Dwclamp(i) = 2;


    elseif (rad2deg(angle(i))<60)
        DPWM3Dusec(i) = +0.5;
        DPWM3Du(i) = 0;
        DPWM3Duclamp(i) = 1;
        
        DPWM3Dvsec(i) = +0.5;
        DPWM3Dv(i) = -0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM3Dvclamp(i) = 0;

        DPWM3Dwsec(i) = +0.5;
        DPWM3Dw(i) = +0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM3Dwclamp(i) = 0;


    elseif (rad2deg(angle(i))<90)
        DPWM3Dusec(i) = +0.5;        
        DPWM3Du(i) = +0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM3Duclamp(i) = 0;
        
        DPWM3Dvsec(i) = +0.5;
        DPWM3Dv(i) = 0;
        DPWM3Dvclamp(i) = +1;

        DPWM3Dwsec(i) = +0.5;
        DPWM3Dw(i) = -0.5*(3^0.5 * M * sin(angle(i)) );
        DPWM3Dwclamp(i) = 0;
    
    elseif (rad2deg(angle(i))<120)
        DPWM3Dusec(i) = -0.5;
        DPWM3Du(i) = -0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM3Duclamp(i) = 0;
        
        DPWM3Dvsec(i) = -0.5;
        DPWM3Dv(i) = +0.5*(3^0.5 * M * sin(angle(i)) );
        DPWM3Dvclamp(i) = 0;

        DPWM3Dwsec(i) = -0.5;
        DPWM3Dw(i) = 0;
        DPWM3Dwclamp(i) = 2;

    elseif (rad2deg(angle(i))<150)
        DPWM3Dusec(i) = -0.5;
        DPWM3Du(i) = 0;
        DPWM3Duclamp(i) = 2;
        
        DPWM3Dvsec(i) = -0.5;
        DPWM3Dv(i) = -0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM3Dvclamp(i) = 0;

        DPWM3Dwsec(i) = -0.5;
        DPWM3Dw(i) = +0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM3Dwclamp(i) = 0;
    
    elseif (rad2deg(angle(i))<180)
        DPWM3Dusec(i) = +0.5;
        DPWM3Du(i) = +0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM3Duclamp(i) = 0;
        
        DPWM3Dvsec(i) = +0.5;
        DPWM3Dv(i) = 0;
        DPWM3Dvclamp(i) = 1;

        DPWM3Dwsec(i) = +0.5;        
        DPWM3Dw(i) = -0.5*(3^0.5 * M * sin(angle(i)) );
        DPWM3Dwclamp(i) = 0;

    elseif (rad2deg(angle(i))<210)
        DPWM3Dusec(i) = +0.5;
        DPWM3Du(i) = -0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM3Duclamp(i) = 0;
        
        DPWM3Dvsec(i) = +0.5;
        DPWM3Dv(i) = +0.5*(3^0.5 * M * sin(angle(i)) );
        DPWM3Dvclamp(i) = 0;

        DPWM3Dwsec(i) = +0.5;
        DPWM3Dw(i) = 0;
        DPWM3Dwclamp(i) = 1;

    elseif (rad2deg(angle(i))<240)
        DPWM3Dusec(i) = -0.5;
        DPWM3Du(i) = 0;
        DPWM3Duclamp(i) = 2;
        
        DPWM3Dvsec(i) = -0.5;
        DPWM3Dv(i) = -0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM3Dvclamp(i) = 0;

        DPWM3Dwsec(i) = -0.5;
        DPWM3Dw(i) = +0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM3Dwclamp(i) = 0; 

    elseif (rad2deg(angle(i))<270)
        DPWM3Dusec(i) = -0.5;
        DPWM3Du(i) = +0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM3Duclamp(i) = 0;
        
        DPWM3Dvsec(i) = -0.5;
        DPWM3Dv(i) = 0;
        DPWM3Dvclamp(i) = 2;

        DPWM3Dwsec(i) = -0.5;
        DPWM3Dw(i) = -0.5*(3^0.5 * M * sin(angle(i)) );
        DPWM3Dwclamp(i) = 0;

    elseif (rad2deg(angle(i))<300)
        DPWM3Dusec(i) = +0.5;
        DPWM3Du(i) = -0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM3Duclamp(i) = 0;
        
        DPWM3Dvsec(i) = +0.5;
        DPWM3Dv(i) = +0.5*(3^0.5 * M * sin(angle(i)) );
        DPWM3Dvclamp(i) = 0;

        DPWM3Dwsec(i) = +0.5;
        DPWM3Dw(i) = 0;
        DPWM3Dwclamp(i) = 1;        

    elseif (rad2deg(angle(i))<330)
        DPWM3Dusec(i) = +0.5;
        DPWM3Du(i) = 0;
        DPWM3Duclamp(i) = 1;
        
        DPWM3Dvsec(i) = +0.5;
        DPWM3Dv(i) = -0.5*(3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM3Dvclamp(i) = 0;

        DPWM3Dwsec(i) = +0.5;
        DPWM3Dw(i) = +0.5*(3^0.5 * M * cos(angle(i)+(5*pi()/6)) );
        DPWM3Dwclamp(i) = 0;        

    else %till 360
        DPWM3Dusec(i) = -0.5;
        DPWM3Du(i) = +0.5* (3^0.5 * M * cos(angle(i)+(pi()/6)) );
        DPWM3Duclamp(i) = 0;
        
        DPWM3Dvsec(i) = -0.5;
        DPWM3Dv(i) = 0;
        DPWM3Dvclamp(i) = 2;

        DPWM3Dwsec(i) = -0.5;
        DPWM3Dw(i) = -0.5* (3^0.5 * M * sin(angle(i)) );
        DPWM3Dwclamp(i) = 0;

    end


end


%%%%%%%%
%END DPWM3 CALCULATION
%%%%%%%%

fixedPointValue = fi(DPWM3Du, 1, 16, 14);
DPWM3Dufi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWM3Dusec, 1, 16, 14);
DPWM3Dusecfi = storedInteger(fixedPointValue);

DPWM3Duclamp(DPWM3Duclamp==-1) = 2;
DPWM3Duclamp = uint8(DPWM3Duclamp);

%%
fixedPointValue = fi(DPWM3Dv, 1, 16, 14);
DPWM3Dvfi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWM3Dvsec, 1, 16, 14);
DPWM3Dvsecfi = storedInteger(fixedPointValue);

DPWM3Dvclamp(DPWM3Dvclamp==-1) = 2;
DPWM3Dvclamp = uint8(DPWM3Dvclamp);

%%

fixedPointValue = fi(DPWM3Dw, 1, 16, 14);
DPWM3Dwfi = storedInteger(fixedPointValue);

fixedPointValue = fi(DPWM3Dwsec, 1, 16, 14);
DPWM3Dwsecfi = storedInteger(fixedPointValue);

DPWM3Dwclamp(DPWM3Dwclamp==-1) = 2;
DPWM3Dwclamp = uint8(DPWM3Dwclamp);


if doplot ==1

%SVPWM.steps = steps;
%SVPWM.angle = angle;
%close all
figure(1)
hold on
title('DPWM3; double; real value')
%plot(angle,sec,'b')
plot(rad2deg(angle),DPWM3Dusec + DPWM3Du,'b')
plot(rad2deg(angle),DPWM3Duclamp,'c--')

plot(rad2deg(angle),DPWM3Dvsec + DPWM3Dv,'r')
plot(rad2deg(angle),DPWM3Dvclamp,'m--')

plot(rad2deg(angle),DPWM3Dwsec + DPWM3Dw,'g')
plot(rad2deg(angle),DPWM3Dwclamp,'y--')


%plot(angle,SVPWM.Dv,'r')
%plot(angle,SVPWM.Dw,'g')
%hl(1) = legend;
grid on
% 
figure
hold on
title('DPWM3; fixed point; stored integer')
plot(1:length(DPWM3Dufi),DPWM3Dufi,'b')
plot(1:length(DPWM3Dvfi),DPWM3Dvfi,'r')
plot(1:length(DPWM3Dwfi),DPWM3Dwfi,'g')
% hl(2) = legend;
% grid on
% 
% hl(1).String = {'U','V','W'};
% hl(2).String = {'U','V','W'};
% 
% 
end
clear fixedPointValue hl steps M i doplot