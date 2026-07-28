import assert from 'node:assert/strict';
import test from 'node:test';

import { nextPackageVersion } from './resolve-npm-version.mjs';

test('increments the highest matching package revision', () => {
  assert.equal(
    nextPackageVersion('1.13.4-espeak-voice', [
      '1.13.4-espeak-voice.3',
      '1.13.4-espeak-voice.5',
      '2.0.0',
      '1.13.4-other.99',
    ]),
    '1.13.4-espeak-voice.6',
  );
});

test('starts at revision zero when the prefix has never been published', () => {
  assert.equal(nextPackageVersion('1.13.4-espeak-voice', []), '1.13.4-espeak-voice.0');
});
