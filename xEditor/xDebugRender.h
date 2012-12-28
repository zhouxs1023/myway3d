#pragma once

class xDebugRender
{
public:
	enum Mode {
		K_Default,
		K_DisplayNormal,
	};

public:
	xDebugRender();
	~xDebugRender();

	void SetMode(Mode m) { mMode = m; }
	Mode GetMode() const { return mMode; }

protected:
	void _init(void *, void *);
	void _shutdown(void *, void *);
	void _render(void *, void *);

protected:
	tEventListener<xDebugRender> OnInit;
	tEventListener<xDebugRender> OnShutdown;
	tEventListener<xDebugRender> OnRender;

	Technique * mTech_Normal;

	Mode mMode;
};
