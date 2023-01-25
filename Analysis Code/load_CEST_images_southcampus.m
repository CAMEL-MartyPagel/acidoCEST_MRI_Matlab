function [images,images_BeforeAvg,SatFrqList,NR,MHz_value,SatFrqList_ppm,CEST_offsets,CEST_rows,CEST_columns,SatPower,SatTime,SpecAvg]=load_CEST_images_southcampus(DIR)
    Home_DIR=pwd;
    cd(DIR);
    
    if exist(fullfile(DIR,'method'),'file')
        methodfile = fullfile(DIR,'method');
    else
        error('Error! \nmethod file does not exist, please check the path %s ',fullfile(DIR,'method'))
    end
    
    method_file=fopen(methodfile,'r');
    
    % Obtain the line number for the Saturtion Frequency List, and also
    % obtain the number of repetitions (NR).
    NR=[];
    line_number=0;
    while 1
        line_number=line_number+1;
        current_line = fgetl(method_file);
        if ~ischar(current_line),  break,   end
        if strncmp(current_line, '##$Method',9)
            sequence = strread(current_line, '##$Method=%s');
            flag_FISP=isempty(strfind(sequence{1,1},'FISP'));
            flag_RARE=isempty(strfind(sequence{1,1},'RARE'));
            if flag_FISP==1 && flag_RARE==1
                error('Error: The sequence selected is not a FISP or RARE. \n The selected CEST scan is: %s', DIR);
            end
        end
        if strncmp(current_line, '##$SatFrqList', 13) || strncmp(current_line, '##$CESTSatFrq', 13) || strncmp(current_line,'##$MT_Offsets_NoM0',18)
            SatFrqList_linestart = line_number;
        end
        if strncmp(current_line,'##$MT_Offsets_NoM0',18)
            SatFrqList_linestart = line_number+1;
        end
        if strncmp(current_line, '##$PVM_NRepetitions', 19)
            NR = strread(current_line,'##$PVM_NRepetitions=%f');
        end
        if strncmp(current_line, '##$PVM_MagTransModuleTime', 25)
            SatTime = strread(current_line,'##$PVM_MagTransModuleTime=%f');
        end
        if strncmp(current_line, '##$PVM_MagTransPower', 20)
            SatPower = strread(current_line, '##$PVM_MagTransPower=%f');
        end
        if strncmp(current_line, '##$PVM_SPackArrSliceOrient', 26)
            CEST_image_orientation=fgetl(method_file);
        end
        if strncmp(current_line, '##$Number_CESTrepetitions', 25)
            SpecAvg = strread(current_line,'##$Number_CESTrepetitions=%f');
        end
    end
    
    fclose(method_file);
    
    % Get the Saturation Frequency List
    SatFrqList_from_method_file = importdata('method',' ',SatFrqList_linestart);
    SatFrqList=SatFrqList_from_method_file.data';
    SatFrqList=SatFrqList(:);
    SatFrqList=SatFrqList(~isnan(SatFrqList));
    
    
    % Get the image matrix dimensions, MHz value,
    % read, slice and axial positions, and read & slice resolutions
    DIR_pdata=fullfile(DIR,'pdata','1');
    cd(DIR_pdata)
    
    if exist(fullfile(DIR_pdata,'visu_pars'),'file')
        visu_parsfile =fullfile(DIR_pdata,'visu_pars');
    else
        error('Error! n\visu_pars file does not exist, please check the path %s ',fullfile(DIR_pdata,'visu_pars'))
    end
    
    visu_pars_file=fopen(visu_parsfile,'r');
    while 1
        current_line = fgetl(visu_pars_file);
        if ~ischar(current_line),   break,   end
        if strncmp(current_line, '##$VisuCoreSize',15)
            current_line=fgetl(visu_pars_file);
            CEST_image_size=strread(current_line);
            CEST_rows=CEST_image_size(2);
            CEST_columns=CEST_image_size(1);
        end
        if strncmp(current_line, '##$VisuAcqImagingFrequency', 26)
            MHz_value = strread(current_line,'##$VisuAcqImagingFrequency=%f');
        end
        if strncmp(current_line, '##$VisuCorePosition',19)
            current_line=fgetl(visu_pars_file);
            CEST_offsets = strread(current_line);
        end
    end
    fclose(visu_pars_file);
    
    SatFrqList_ppm = round(SatFrqList./MHz_value,2);
    
    
    % Get the image data from the 2dseq file.
    cd(DIR_pdata);
    
    if exist(fullfile(DIR_pdata,'2dseq'),'file')
        imagefile = fullfile(DIR_pdata,'2dseq');
    else
        error('Error! n\2dseq file does not exist, please check the path %s ',fullfile(DIR_pdata,'2dseq'))
    end
    
    FileID = fopen(imagefile,'r','l');
    image_from_2dseq = fread(FileID,'int16');
    fclose(FileID);
    
    % reshape the list of data in 2dseq into a 4D matrix
    NumSatFrqTotal=length(SatFrqList);
    images_BeforeAvg=reshape(image_from_2dseq,CEST_columns,CEST_rows,NumSatFrqTotal*SpecAvg,1);
    images_BeforeAvg=permute(images_BeforeAvg,[2 1 3 4]);
    
    % Averaging the images acquired for multiple CEST repetitions
    if SpecAvg >= 2
    NumSatFrqTotal=length(SatFrqList);
    images=zeros(CEST_rows,CEST_columns,NumSatFrqTotal);
    for aaa=1:NumSatFrqTotal
        images(:,:,aaa)=sum(images_BeforeAvg(:,:,aaa:length(SatFrqList_ppm):(NumSatFrqTotal*SpecAvg)),3)./SpecAvg;
    end
    else images=images_BeforeAvg;
    end
    
    
    cd(Home_DIR)
    end