classdef invADC < matlab.System & coder.ExternalDependency
    %
    % Inverter ADC for Arudino Nano 33 IoT
    % Read 4 channels in sequence sync with TCC1 timer
    
    
    properties
        % Public, tunable properties.
    end
    
    properties (Nontunable)
        % Public, non-tunable properties.

    end
    
    properties (Access = private)
        % Pre-computed constants.
    end
    
    methods
        % Constructor
        function obj = invADC(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end
    end
    
    methods (Access=protected)

        function setupImpl(obj) 
            if isempty(coder.target)
                % Place simulation setup code here
            else
                % Call C-function implementing device initialization
   
                 coder.cinclude('invADC.h');
                 coder.cinclude('invPWM.h');
                 coder.cinclude('invClockGen.h');
                 coder.ceval('invClockGen_init'); %ADC needs clock
                 coder.ceval('invPWM_init'); %ADC needs PWM functions due to Event System trigger the ADC start
                 coder.ceval('invADC_init'); 

            end
        end
        
        function [ADC_IphU,ADC_IphV,ADC_IphW,ADC_IrefW] = stepImpl(obj)  
                ADC_IphU = uint16(0);
                ADC_IphV = uint16(0);
                ADC_IphW = uint16(0);
                ADC_IrefW = uint16(0);
            if isempty(coder.target)
                % Place simulation output code here 
            else
                % Call C-function implementing device output
                ADC_IphU = coder.ceval('invADC_read',uint8(0));                
                ADC_IphV = coder.ceval('invADC_read',uint8(1));                
                ADC_IphW = coder.ceval('invADC_read',uint8(2));                
                ADC_IrefW = coder.ceval('invADC_read',uint8(3));                
            end
        end
        
        function releaseImpl(obj) 
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                %coder.ceval('sink_terminate');
            end
        end
    end
    
    methods (Access=protected)
        %% Define input properties
        function num = getNumInputsImpl(~)
            num = 0;
        end
        
        function num = getNumOutputsImpl(~)
            num = 4;
        end
        
        function varargout = isOutputFixedSizeImpl(~,~)
            varargout{1} = true;
            varargout{2} = true;
            varargout{3} = true;
            varargout{4} = true;
        end
        
        function varargout = isOutputComplexImpl(~)
            varargout{1} = false;
            varargout{2} = false;
            varargout{3} = false;
            varargout{4} = false;
        end
        
        function varargout = getOutputSizeImpl(~)
            varargout{1} = [1,1];
            varargout{2} = [1,1];
            varargout{3} = [1,1];
            varargout{4} = [1,1];
        end

        function varargout = getOutputDataTypeImpl(~)
            varargout{1} = 'uint16';
            varargout{2} = 'uint16';
            varargout{3} = 'uint16';
            varargout{4} = 'uint16';
        end
        
        function flag = isInputSizeMutableImpl(~,~)
            flag = false;
        end
        
        function flag = isInputComplexityMutableImpl(~,~)
            flag = false;
        end
        
        function validateInputsImpl(~)
            if isempty(coder.target)
                % Run input validation only in Simulation

            end
        end
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = 'Inverter ADC Arduino Nano 33 IoT';
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
            name = 'Inverter ADC Read';
        end
        
        function b = isSupportedContext(context)
            b = context.isCodeGenTarget('rtw');
        end
        
        function updateBuildInfo(buildInfo, context)
            if context.isCodeGenTarget('rtw')
                % Update buildInfo
                srcDir = fullfile(fileparts(mfilename('fullpath')),'src'); 
                includeDir = fullfile(fileparts(mfilename('fullpath')),'include');
                addIncludePaths(buildInfo,includeDir);
                % Use the following API's to add include files, sources and
                % linker flags
                %addIncludeFiles(buildInfo,'source.h',includeDir);
                addSourceFiles(buildInfo,'invADC.cpp',srcDir);
                %addLinkFlags(buildInfo,{'-lSource'});
                %addLinkObjects(buildInfo,'sourcelib.a',srcDir);
                %addCompileFlags(buildInfo,{'-D_DEBUG=1'});
                %addDefines(buildInfo,'MY_DEFINE_1')
            end
        end
    end
end
