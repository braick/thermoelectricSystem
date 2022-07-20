<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="21008000">
	<Property Name="varPersistentID:{15B525B2-CAF5-4716-B6E6-8B55B692F6A1}" Type="Ref">/My Computer/globalVars.lvlib/sensD</Property>
	<Property Name="varPersistentID:{1887FF1B-6045-47A5-94A1-C3E30B9F42A3}" Type="Ref">/My Computer/globalVars.lvlib/state</Property>
	<Property Name="varPersistentID:{8B84199B-D890-45B4-89CF-E0B2B27F5B7C}" Type="Ref">/My Computer/globalVars.lvlib/loopsStop</Property>
	<Property Name="varPersistentID:{ED14101D-2C02-42B1-8C32-20983CD79495}" Type="Ref">/My Computer/globalVars.lvlib/sensDat</Property>
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
		<Item Name="globalVars.lvlib" Type="Library" URL="../globalVars.lvlib"/>
		<Item Name="main.vi" Type="VI" URL="../main.vi"/>
		<Item Name="sesnorData.ctl" Type="VI" URL="../sesnorData.ctl"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="FormatTime String.vi" Type="VI" URL="/&lt;vilib&gt;/express/express execution control/ElapsedTimeBlock.llb/FormatTime String.vi"/>
				<Item Name="subElapsedTime.vi" Type="VI" URL="/&lt;vilib&gt;/express/express execution control/ElapsedTimeBlock.llb/subElapsedTime.vi"/>
			</Item>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
