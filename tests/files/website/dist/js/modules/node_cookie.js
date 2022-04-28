const writeCookie = (name, value) =>
{
	if (!name || !value)
		return true;
	const date = new Date(Date.now() + 86400e3).toUTCString();
	document.cookie = `${String(name)}=${String(value)}; expires=${date}`;
	return false;
};

const deleteCookie = (name) =>
{
	if (!name)
		return true;
	let date = new Date(Date.now());
	date.setMonth(date.getMonth() - 1).toUTCString();
	document.cookie = `${String(name)}=${String("")}; expires=${date}`;
	return false;
};

const getCookies = () =>
{
	const ret = [];
	const cookies = document.cookie.split(';');
	
	for (let i = 0; i < cookies.length; i++)
	{
		const temp = cookies[i].split('=');
		ret.push({
			name: temp[0],
			value: temp[1],
		});
	}
	return ret;
};

const getCookie = (name) =>
{
	const arr = getCookies();
	if (!arr.length)
		return undefined;
	for (const cookie of arr)
	{
		if (cookie.name === name)
			return cookie;
	}
	return undefined;
};

window.addEventListener('DOMContentLoaded', () =>
{
	const __COOKIE_NAME = String('daisy_js');
	const __COOKIE = getCookie(__COOKIE_NAME);

	if (__COOKIE === undefined)
	{
		writeCookie(__COOKIE_NAME, String(1));
		document.getElementById('cookie_value').innerText = String(1);
	}
	else
	{
		let data = Number(__COOKIE.value);
		++data;
		writeCookie(__COOKIE_NAME, String(data));
		document.getElementById('cookie_value').innerText = String(data);
	}
});
