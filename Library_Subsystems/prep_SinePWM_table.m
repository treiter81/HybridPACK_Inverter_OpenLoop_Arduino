%clear 
doplot=1;

steps = 1024;
angle = linspace(0,2*pi,steps+1);
angle = angle(1:end-1);

SPWMDu = zeros(steps,1);
SPWMDv = zeros(steps,1);
SPWMDw = zeros(steps,1);


for i=1:length(angle)
    M=1;
    
    SPWMDu(i) = sin(angle(i)+0.5*pi()) * M/2;
    SPWMDv(i) = sin(angle(i)+0.5*pi()+(4*pi()/3)) * M/2;
    SPWMDw(i) = sin(angle(i)+0.5*pi()+(2*pi()/3)) * M/2;
end

%Du = fixdt(1,16,2)
fixedPointValue = fi(SPWMDu, 1, 16, 14);
SPWMDufi = storedInteger(fixedPointValue);

fixedPointValue = fi(SPWMDv, 1, 16, 14);
SPWMDvfi = storedInteger(fixedPointValue);

fixedPointValue = fi(SPWMDw, 1, 16, 14);
SPWMDwfi = storedInteger(fixedPointValue);

%SPWMsteps = steps;
%SPWMangle = angle;

if doplot ==1

figure
hold on
title('Sine PWM; double; real value')
plot(rad2deg(angle),SPWMDu,'b')
plot(rad2deg(angle),SPWMDv,'r')
plot(rad2deg(angle),SPWMDw,'g')
hl(1) = legend;
grid on

figure
hold on
title('Sine PWM; fixed point; stored integer')
plot(1:length(SPWMDufi),SPWMDufi,'b')
plot(1:length(SPWMDvfi),SPWMDvfi,'r')
plot(1:length(SPWMDwfi),SPWMDwfi,'g')
hl(2) = legend;
grid on

hl(1).String = {'U','V','W'};
hl(2).String = {'U','V','W'};
end
clear fixedPointValue hl steps M i doplot