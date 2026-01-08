classdef invPWM_DutyPeriod < matlab.System & coder.ExternalDependency
    % Inverter PWM for Arudino Nano 33 IoT
    % Enable, Freqency, Duty, Dead Time
    properties
        % Public, tunable properties.
    end
    
    properties (Nontunable)
        % Init period of PWM
    end
    
    properties (Access = private)
        % Pre-computed constants.
    end
    
    methods
        % Constructor
        function obj = invPWM(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end
    end
    
    methods (Access=protected)
%        function setupImpl(obj, fsw, dT) %#ok<MANU>
        function setupImpl(obj) %#ok<MANU>

            if isempty(coder.target)%||coder.BuildConfig.isMatlabHostTarget
                % Place simulation setup code here              
            else
                % Call C-function implementing device initialization
                %PWM needs clock first
                coder.cinclude('invClockGen.h');
                coder.ceval('invClockGen_init');
                %PWM Init
                coder.cinclude('invPWM.h');
                coder.ceval('invPWM_init');                 
            end
        end
        
        function stepImpl(obj, Period, Du, Dv, Dw)  %#ok<INUSD>
                
            if isempty(coder.target)%||coder.BuildConfig.isMatlabHostTarget
                % Place simulation output code here 
            else
                % Call C-function implementing device output                
                coder.ceval('invPWM_dutycycle',uint16(Period), uint16(Du), uint16(Dv), uint16(Dw));                
            end
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)%||coder.BuildConfig.isMatlabHostTarget
                % Place simulation termination code here
            else
                % Call C-function implementing device termination                
            end
        end
    end
    
    methods (Access=protected)
        %% Define input properties
        function num = getNumInputsImpl(~)
            num = 4;
        end
        
        function num = getNumOutputsImpl(~)
            num = 0;
        end
        
        %function varargout = isOutputFixedSizeImpl(~,~)
        %    varargout{1} = true;
        %end
        
        %function varargout = isOutputComplexImpl(~)
        %    varargout{1} = false;
        %end
        
        %function varargout = getOutputSizeImpl(~)
        %    varargout{1} = [1,1];
        %end

        %function varargout = getOutputDataTypeImpl(~)
        %    varargout{1} = 'uint8';
        %end
        
        function flag = isInputSizeMutableImpl(~,~)
            flag = false;
        end
        
        function flag = isInputComplexityMutableImpl(~,~)
            flag = false;
        end
        
        function validateInputsImpl(~, Period, Du, Dv, Dw)
            if isempty(coder.target)
                % Run input validation only in Simulation
                validateattributes(Period,{'uint16'},{'scalar'},'','Period');
                validateattributes(Du,{'uint16'},{'scalar'},'','Du');
                validateattributes(Dv,{'uint16'},{'scalar'},'','Dv');
                validateattributes(Dw,{'uint16'},{'scalar'},'','Dw');
            end
        end
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = 'Inverter PWM Arduino Nano 33 IoT';
        end
    end
    
    methods (Static, Access=protected)
        function simMode = getSimulateUsingImpl(~)
            simMode = 'Interpreted execution';
        end
        
        function isVisible = showSimulateUsingImpl
            isVisible = false;
        end
    end
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'Inverter PWM DutyCycles';
        end
        
        function b = isSupportedContext(context)
            b = context.isCodeGenTarget('rtw');
        end
        
        function updateBuildInfo(buildInfo, context)

            if context.isCodeGenTarget('rtw')                
                % Update buildInfo for Arduino Code
                srcDir = fullfile(fileparts(mfilename('fullpath')),'src');
                includeDir = fullfile(fileparts(mfilename('fullpath')),'include');
                addIncludePaths(buildInfo,includeDir);
                % Use the following API's to add include files, sources and
                % linker flags
                %addIncludeFiles(buildInfo,'source.h',includeDir);
                addSourceFiles(buildInfo,'invPWM.cpp',srcDir);
                %addLinkFlags(buildInfo,{'-lSource'});
                %addLinkObjects(buildInfo,'sourcelib.a',srcDir);
                %addCompileFlags(buildInfo,{'-D_DEBUG=1'});
                %addDefines(buildInfo,'MY_DEFINE_1')
            end
        end
    end
end
