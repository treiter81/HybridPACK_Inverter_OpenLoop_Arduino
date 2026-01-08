classdef invIOfastTX0 < matlab.System & coder.ExternalDependency
    % Inverter IO Pins for Arudino Nano 33 IoT
    % TX0, RX1    
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
        function obj = invIOfastTX0(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end
    end
    
    methods (Access=protected)
%        function setupImpl(obj, fsw, dT) %#ok<MANU>
        function setupImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation setup code here
            else
                % Call C-function implementing device initialization
                 coder.cinclude('invIOfast.h');
                 coder.ceval('Pin0TX_init');
            end
        end
        
        function stepImpl(obj, level)  %#ok<INUSD>
                
            if isempty(coder.target)
                % Place simulation output code here 
            else
                % Call C-function implementing device output
                coder.ceval('Pin0TX_set',level);                
            end
        end
        
        function releaseImpl(obj) %#ok<MANU>
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
            num = 1;
        end
        
        function num = getNumOutputsImpl(~)
            num = 0;
        end
        
  %      function varargout = isOutputFixedSizeImpl(~,~)
  %          varargout{1} = true;
   %     end
        
 %       function varargout = isOutputComplexImpl(~)
 %           varargout{1} = false;
 %       end
        
%        function varargout = getOutputSizeImpl(~)
%            varargout{1} = [1,1];
%        end

%        function varargout = getOutputDataTypeImpl(~)
%            varargout{1} = 'uint8';
%        end
        
        function flag = isInputSizeMutableImpl(~,~)
            flag = false;
        end
        
        function flag = isInputComplexityMutableImpl(~,~)
            flag = false;
        end
        
        function validateInputsImpl(~, pegel)
            if isempty(coder.target)
                % Run input validation only in Simulation
                validateattributes(pegel,{'uint8'},{'scalar'},'','IO Level');
            end
        end
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = 'Arduino Nano 33 IoT \n IO 0 TX FAST';
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
            name = 'IO 0 TX FAST';
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
                addSourceFiles(buildInfo,'invIOfast.cpp',srcDir);
                %addLinkFlags(buildInfo,{'-lSource'});
                %addLinkObjects(buildInfo,'sourcelib.a',srcDir);
                %addCompileFlags(buildInfo,{'-D_DEBUG=1'});
                %addDefines(buildInfo,'MY_DEFINE_1')
            end
        end
    end
end
