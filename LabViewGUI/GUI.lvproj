<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="21008000">
	<Property Name="varPersistentID:{1887FF1B-6045-47A5-94A1-C3E30B9F42A3}" Type="Ref">/My Computer/globalVars.lvlib/state</Property>
	<Property Name="varPersistentID:{2C595DA2-D28F-46DC-8ECB-26B3A332A0AD}" Type="Ref">/My Computer/globalVars.lvlib/press</Property>
	<Property Name="varPersistentID:{62E55119-BD7E-48CF-9E97-DE38A7BC701D}" Type="Ref">/My Computer/globalVars.lvlib/sensDt</Property>
	<Property Name="varPersistentID:{8B84199B-D890-45B4-89CF-E0B2B27F5B7C}" Type="Ref">/My Computer/globalVars.lvlib/loopsStop</Property>
	<Property Name="varPersistentID:{D140D339-AE5A-402F-B5C2-55CA1D6C5BE2}" Type="Ref">/My Computer/globalVars.lvlib/saveData</Property>
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="callResponse.vi" Type="VI" URL="../callResponse.vi"/>
		<Item Name="CANFrame2SensorValVI.vi" Type="VI" URL="../CANFrame2SensorValVI.vi"/>
		<Item Name="fullPath.vi" Type="VI" URL="../fullPath.vi"/>
		<Item Name="globalVars.lvlib" Type="Library" URL="../globalVars.lvlib"/>
		<Item Name="linFit.vi" Type="VI" URL="../linFit.vi"/>
		<Item Name="linFitArrayPress.vi" Type="VI" URL="../linFitArrayPress.vi"/>
		<Item Name="main.vi" Type="VI" URL="../main.vi"/>
		<Item Name="saveSensorsData.vi" Type="VI" URL="../saveSensorsData.vi"/>
		<Item Name="sesnorData.ctl" Type="VI" URL="../sesnorData.ctl"/>
		<Item Name="Untitled 1.vi" Type="VI" URL="../../../../Documents/Untitled 1.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="ex_CorrectErrorChain.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/ex_CorrectErrorChain.vi"/>
				<Item Name="FormatTime String.vi" Type="VI" URL="/&lt;vilib&gt;/express/express execution control/ElapsedTimeBlock.llb/FormatTime String.vi"/>
				<Item Name="subElapsedTime.vi" Type="VI" URL="/&lt;vilib&gt;/express/express execution control/ElapsedTimeBlock.llb/subElapsedTime.vi"/>
				<Item Name="subFile Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/express/express input/FileDialogBlock.llb/subFile Dialog.vi"/>
				<Item Name="Write Delimited Spreadsheet (DBL).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write Delimited Spreadsheet (DBL).vi"/>
				<Item Name="Write Delimited Spreadsheet (I64).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write Delimited Spreadsheet (I64).vi"/>
				<Item Name="Write Delimited Spreadsheet (string).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write Delimited Spreadsheet (string).vi"/>
				<Item Name="Write Delimited Spreadsheet.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write Delimited Spreadsheet.vi"/>
				<Item Name="Write Spreadsheet String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write Spreadsheet String.vi"/>
			</Item>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
