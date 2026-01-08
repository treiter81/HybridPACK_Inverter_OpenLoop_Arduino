classdef invSPI < matlab.System & coder.ExternalDependency
    % Inverter SPI for Arudino Nano 33 IoT
    % Read 16 bit data from external ADC via SPI. CS is used from PWM driver. Use this SPI only within the Interrupt Routine of invPWM Driver
    
    properties
        % Public, tunable properties.
    end
    
    properties (Nontunable)

    end
    
    properties (Access = private)
        % Pre-computed constants.
    end
    
    methods
        % Constructor
        function obj = invSPI(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end
    end
    
    methods (Access=protected)
%        function setupImpl(obj, fsw, dT) 
        function setupImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation setup code here
            else
                % Call C-function implementing device initialization
                 coder.cinclude('invSPI.h');
                 coder.cinclude('invClockGen.h');
                 coder.cinclude('invPWM.h');
                 coder.ceval('invClockGen_init');
                 %coder.ceval('invPWM_init'); 
                 coder.ceval('invSPI_init');

            end
        end
        
        function [DCL_RAW] = stepImpl(obj)  
                DCL_RAW = uint16(0);
            if isempty(coder.target)
                % Place simulation output code here 
            else
                % Call C-function implementing device output
                DCL_RAW = coder.ceval('readwriteSPI',uint8(0),uint8(1));                
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
            num = 1;
        end
        
        function varargout = isOutputFixedSizeImpl(~,~)
            varargout{1} = true;
        end
        
        function varargout = isOutputComplexImpl(~)
            varargout{1} = false;
        end
        
        function varargout = getOutputSizeImpl(~)
            varargout{1} = [1,1];
        end

        function varargout = getOutputDataTypeImpl(~)
            varargout{1} = 'uint16';
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
            icon = 'Inverter SPI Arduino Nano 33 IoT';
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
            name = 'Inverter SPI external ADC Read';
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
                addSourceFiles(buildInfo,'invSPI.cpp',srcDir);
                %addLinkFlags(buildInfo,{'-lSource'});
                %addLinkObjects(buildInfo,'sourcelib.a',srcDir);
                %addCompileFlags(buildInfo,{'-D_DEBUG=1'});
                %addDefines(buildInfo,'MY_DEFINE_1')
            end
        end
    end
end
